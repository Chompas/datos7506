/*
 * BKDManagerDisco.cpp
 *
 *  Created on: May 28, 2012
 *      Author: vdiego
 */

#include <iostream>
#include "BKDManagerDisco.h"
#include "BKDNodoHojaDisco.h"
#include "BKDNodoInternoDisco.h"
#include "../Disco/Registro.h"
#include "../Disco/Bloque.h"


using namespace std;

typedef struct
{
	int Nivel;

} RegControlNodo;


//************ METODOS PRIVADOS ******************//


RegControlNodo LeerRegistroControlNodo(Registro* registro)
{
	RegControlNodo rc;

	if (registro != NULL)
	{
		//**TODO: Cargar El Nivel

	}

	return rc;
}

Registro* GuardarRegistroControlNodo(RegControlNodo regControl)
{
	//** TODO:

	return NULL;
}


BKDNodo* BKDManagerDisco::GetCopiaNodo(BKDNodo* nodo)
{
	if (nodo == NULL)
		return NULL;
	else if (nodo->EsNodoHoja())
		return new BKDNodoHojaDisco((const BKDNodoHojaDisco&)(*nodo));
	else
		return new BKDNodoInternoDisco((const BKDNodoInternoDisco&)(*nodo));
}


void BKDManagerDisco::RefrescarRaiz()
{
	if (this->m_raiz != NULL)
	{
		delete this->m_raiz;
		this->m_raiz = NULL;
	}

	this->m_raiz = this->GetNodo(this->m_raiz_id);
}

int NroNodoToNumBloque(int nroNodo)
{
	return nroNodo - 1;
}

int NroBloqueToNroNodo(int nroBloque)
{
	return nroBloque + 1;
}

//***********************************************//

BKDManagerDisco::BKDManagerDisco(const string& filePath, const int tamanioBytesBloque)
{
	this->m_raiz_id = 1; //La raiz siempre es el nodo 1 de datos
	this->m_raiz = NULL;

	this->m_capacidad_hoja = tamanioBytesBloque;
	this->m_capacidad_interno = tamanioBytesBloque;

	if (filePath.empty())
			cerr << "Debe especificarse un archivo para el indice" << endl;

	if (tamanioBytesBloque < ArchivoBloques::GetTamanioMinimoBloque())
			cerr << "Tamanio de Bloque insuficiente" << endl;

	this->m_arch = new ArchivoBloques(filePath, tamanioBytesBloque);

	//Cargo la raiz
	RefrescarRaiz();
}

BKDManagerDisco::BKDManagerDisco(const std::string& filePath)
{
	this->m_raiz_id = 1; //La raiz siempre es el nodo 1 de datos
	this->m_raiz = NULL;

	if (filePath.empty())
			cerr << "Debe especificarse un archivo para el indice" << endl;

	this->m_arch = new ArchivoBloques(filePath);

	this->m_capacidad_hoja = this->m_arch->GetTamanioDatosBloque();
	this->m_capacidad_interno = this->m_capacidad_hoja;

	//Cargo la raiz
	RefrescarRaiz();
}

BKDManagerDisco::~BKDManagerDisco()
{
	if (this->m_raiz != NULL)
		delete this->m_raiz;

	if (this->m_arch != NULL)
		delete this->m_arch;

}

BKDNodo* BKDManagerDisco::GetNodoRaiz()
{
	if (this->m_raiz == NULL)
		this->RefrescarRaiz();

	return GetCopiaNodo(this->m_raiz);
}

BKDNodo* BKDManagerDisco::GetNodoPrimeraHoja()
{
	//devuelvo la primera hoja, o NULL si no existe
	return NULL; //TODO
}

BKDNodo* BKDManagerDisco::GetNodo(int nroNodo)
{
	//voy a disco a obtener el nodo indicado.
	int nroBloque = NroNodoToNumBloque(nroNodo);
	Bloque* bl = m_arch->GetBloque(nroBloque);

	if (bl == NULL)
	{
		cerr << "Error: No se hallo el nodo " << nroNodo << endl;
		return NULL;
	}

	Registro* regCtrl = bl->obtenerRegistroDeControl();

	if (regCtrl == NULL)
	{
		cerr << "Error: Información de control corrupta en nodo " << nroNodo << endl;
		return NULL;
	}

	RegControlNodo rcn = LeerRegistroControlNodo(regCtrl);
	delete regCtrl;

	BKDNodo* nodo = NULL;

	if (rcn.Nivel == 0)
	{
		BKDNodoHojaDisco* nodoHoja = new BKDNodoHojaDisco(this, nroNodo, this->m_capacidad_hoja, rcn.Nivel);

		if (!nodoHoja->LeerDeBloque(bl))
			cerr << "Error al leer nodo desde Bloque. Nro de bloque: " << nroBloque << endl;
		else
			nodo = nodoHoja;
	}
	else if (rcn.Nivel > 0)
	{
		BKDNodoInternoDisco* nodoInterno = new BKDNodoInternoDisco(this, nroNodo, this->m_capacidad_interno, rcn.Nivel);

		if (!nodoInterno->LeerDeBloque(bl))
			cerr << "Error al leer nodo desde Bloque. Nro de bloque: " << nroBloque << endl;
		else
			nodo = nodoInterno;
	}

	delete bl;

	if (nodo == NULL)
		cerr << "Error: Información de nivel corrupta en nodo " << nroNodo << endl;

	return nodo;
}

BKDNodo* BKDManagerDisco::CrearNodoOffline(int nivel)
{
	//Nodo hoja o intermedio?
	if (nivel == 0)
		return new BKDNodoHojaDisco(this, -1, this->m_capacidad_hoja, nivel);
	else if (nivel > 0)
		return new BKDNodoInternoDisco(this, -1, this->m_capacidad_interno, nivel);
	else
		cerr << "Error al crear nodo offline: nivel inválido" << endl;

	return NULL;
}

BKDNodo* BKDManagerDisco::AgregarNodo(int nivel)
{
	if (nivel < 0)
	{
		cerr << "Error al intentar agregar un nodo: el argumento 'nivel' es inválido." << endl;
		return NULL;
	}

	int nroBloque = -1;
	Bloque* bl = m_arch->AgregarBloque(nroBloque);
	BKDNodo* nodo = NULL;

	if (bl == NULL)
	{
		cerr << "Error al intentar agregar un nodo al archivo." << endl;
		return NULL;
	}
	else if (nroBloque < 0)
		cerr << "Error al intentar agregar un nodo al archivo, identificador invalido." << endl;

	int nroNodo = NroBloqueToNroNodo(nroBloque);
	RegControlNodo rcn;
	rcn.Nivel = nivel;

	Registro* regCtrl = GuardarRegistroControlNodo(rcn);

	if (!bl->insertarRegistroDeControl(regCtrl))
		cerr << "Error al intentar insertar registro de control de nodo." << endl;

	delete regCtrl;

	if (nivel == 0)
	{
		BKDNodoHojaDisco* nodoHoja = new BKDNodoHojaDisco(this, nroNodo, this->m_capacidad_hoja, nivel);

		if (nodoHoja->EscribirEnBloque(bl))
			nodo = nodoHoja;
		else
			cerr << "Error al escribir nodo a Bloque. Nro de nodo: " << nroNodo << endl;
	}
	else if (nivel > 0)
	{
		BKDNodoInternoDisco* nodoInterno = new BKDNodoInternoDisco(this, nroNodo, this->m_capacidad_interno, nivel);

		if (nodoInterno->EscribirEnBloque(bl))
			nodo = nodoInterno;
		else
			cerr << "Error al escribir nodo a Bloque. Nro de nodo: " << nroNodo << endl;
	}

	delete bl;
	return nodo;
}

bool BKDManagerDisco::GuardarNodo(BKDNodo* nodo)
{
	bool res = false;

	if (nodo == NULL)
	{
		cerr << "Error al intentar guardar nodo: el argumento es NULL." << endl;
		return false;
	}

	//voy a disco a obtener el bloque indicado.
	int nroBloque = NroNodoToNumBloque(nodo->GetNumeroNodo());

	if (nroBloque < 0)
			cerr << "Error al intentar guardar nodo: identificador invalido." << endl;

	Bloque* bl = m_arch->GetBloque(nroBloque);

	if (bl == NULL)
	{
		cerr << "Error: No se hallo el bloque " << nroBloque << endl;
		return false;
	}

	RegControlNodo rcn;
	rcn.Nivel = nodo->GetNivel();
	Registro* regCtrl = GuardarRegistroControlNodo(rcn);

	if (!bl->actualizarRegistroDeControl(regCtrl))
	{
		cerr << "Error al intentar actualizar registro de control en bloque " << nroBloque << endl;
		res = false;
	}

	delete regCtrl;

	if (nodo->GetNivel() == 0)
	{
		BKDNodoHojaDisco* nodoHoja = (BKDNodoHojaDisco*)nodo;

		if (nodoHoja->EscribirEnBloque(bl))
			res = true;
		else
			cerr << "Error al escribir nodo a Bloque. Nro de nodo: " << nodo->GetNumeroNodo() << endl;
	}
	else if (nodo->GetNivel() > 0)
	{
		BKDNodoInternoDisco* nodoInterno = (BKDNodoInternoDisco*)nodo;

		if (nodoInterno->EscribirEnBloque(bl))
			res = true;
		else
			cerr << "Error al escribir nodo a Bloque. Nro de nodo: " << nodo->GetNumeroNodo() << endl;
	}

	delete bl;

	if (nodo->GetNumeroNodo() == this->m_raiz_id)
			RefrescarRaiz();

	return res;
}

bool BKDManagerDisco::BorrarNodo(int nroNodo)
{
	//** TODO
	return false;
}
