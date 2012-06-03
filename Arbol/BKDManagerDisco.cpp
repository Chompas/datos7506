/*
 * BKDManagerDisco.cpp
 *
 *  Created on: May 28, 2012
 *      Author: vdiego
 */

#include "BKDManagerDisco.h"

#include <iostream>
#include <cstring>

#include "BKDNodoHojaDisco.h"
#include "BKDNodoInternoDisco.h"

#include "../Comun/Utils.h"
#include "../Disco/Registro.h"
//#include "../Disco/RegistroDeLongitudFija.h"
#include "../Disco/RegistroDeLongitudVariable.h"
#include "../Disco/Bloque.h"

namespace UT = Utils;

using namespace std;

typedef struct
{
	int Nivel;

} RegControlNodo;


//************ METODOS PRIVADOS ******************//


RegControlNodo LeerRegistroControlNodo(Registro* registro)
{
	RegControlNodo rc;
	rc.Nivel = -1;

	if (registro != NULL)
	{
		int tam = 0;
		char* data = registro->getDato(tam);

		if (tam == sizeof(int))
			memcpy(&rc.Nivel, data, tam);
		else
			UT::LogError(UT::errSS << "Error al leer el registro de control para el nodo");

		delete data;
	}

	return rc;
}

Registro* GuardarRegistroControlNodo(RegControlNodo regControl)
{
	//guardo el nivel del nodo
	//Registro* reg = new RegistroDeLongitudFija((char*)(&regControl.Nivel), sizeof(int), sizeof(int));
	//**RegistroDeLongitudFija* reg = new RegistroDeLongitudFija((char*)(&regControl.Nivel), sizeof(int), sizeof(int));
	RegistroDeLongitudVariable* reg = new RegistroDeLongitudVariable((char*)(&regControl.Nivel), sizeof(int));
	return reg;
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

BKDManagerDisco::BKDManagerDisco(const string& filePath, const int tamanioBytesBloque, BKDInstanciador* instanciadorRegistros)
{
	this->m_raiz_id = 1; //La raiz siempre es el nodo 1 de datos
	this->m_raiz = NULL;

	this->m_capacidad_hoja = tamanioBytesBloque;
	this->m_capacidad_interno = tamanioBytesBloque;


	if (filePath.empty())
	{
		UT::LogError(UT::errSS << "Debe especificarse un archivo para el indice");
		return; //throw ex?
	}

	if (tamanioBytesBloque < ArchivoBloques::GetTamanioMinimoBloque())
	{
		UT::LogError(UT::errSS << "Tamanio de Bloque insuficiente");
		return; //throw ex?
	}

	if (instanciadorRegistros == NULL)
	{
		UT::LogError(UT::errSS << "Debe especificarse un instanciador de registros");
		return; //throw ex?
	}

	this->m_instanciador = instanciadorRegistros;

	//creo el archivo
	this->m_arch = new ArchivoBloques(filePath, tamanioBytesBloque);

	//creo el nodo raiz (ya lo guarda en disco)
	this->m_raiz = this->AgregarNodo(0);

	//Recargo la raiz a memoria
	RefrescarRaiz();
}

BKDManagerDisco::BKDManagerDisco(const std::string& filePath, BKDInstanciador* instanciadorRegistros)
{
	this->m_raiz_id = 1; //La raiz siempre es el nodo 1 de datos
	this->m_raiz = NULL;

	if (filePath.empty())
	{
		UT::LogError(UT::errSS << "Debe especificarse un archivo para el indice");
		return; //throw ex?
	}

	if (instanciadorRegistros == NULL)
	{
		UT::LogError(UT::errSS << "Debe especificarse un instanciador de registros");
		return; //throw ex?
	}

	this->m_instanciador = instanciadorRegistros;

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

	if (this->m_instanciador != NULL)
		delete this->m_instanciador;
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
		UT::LogError(UT::errSS << "Error: No se hallo el nodo " << nroNodo);
		return NULL;
	}

	Registro* regCtrl = bl->obtenerRegistroDeControl();

	if (regCtrl == NULL)
	{
		UT::LogError(UT::errSS << "Error: Información de control corrupta en nodo " << nroNodo);
		return NULL;
	}

	RegControlNodo rcn = LeerRegistroControlNodo(regCtrl);
	delete regCtrl;

	BKDNodo* nodo = NULL;

	if (rcn.Nivel == 0)
	{
		BKDNodoHojaDisco* nodoHoja = new BKDNodoHojaDisco(this, nroNodo, this->m_capacidad_hoja, rcn.Nivel);

		if (!nodoHoja->LeerDeBloque(bl))
			UT::LogError(UT::errSS << "Error al leer nodo desde Bloque. Nro de bloque: " << nroBloque);
		else
			nodo = nodoHoja;
	}
	else if (rcn.Nivel > 0)
	{
		BKDNodoInternoDisco* nodoInterno = new BKDNodoInternoDisco(this, nroNodo, this->m_capacidad_interno, rcn.Nivel);

		if (!nodoInterno->LeerDeBloque(bl))
			UT::LogError(UT::errSS << "Error al leer nodo desde Bloque. Nro de bloque: " << nroBloque);
		else
			nodo = nodoInterno;
	}
	else
		UT::LogError(UT::errSS << "Error: Información de nivel corrupta en nodo " << nroNodo);

	delete bl;

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
		UT::LogError(UT::errSS << "Error al crear nodo offline: nivel inválido");

	return NULL;
}

BKDRegistro* BKDManagerDisco::InstanciarRegistro()
{
	return this->m_instanciador->InstanciarRegistro();
}

BKDNodo* BKDManagerDisco::AgregarNodo(int nivel)
{
	if (nivel < 0)
	{
		UT::LogError(UT::errSS << "Error al intentar agregar un nodo: el argumento 'nivel' es inválido.");
		return NULL;
	}

	int nroBloque = -1;
	Bloque* bl = m_arch->AgregarBloque(nroBloque);
	BKDNodo* nodo = NULL;

	if (bl == NULL)
	{
		UT::LogError(UT::errSS << "Error al intentar agregar un nodo al archivo.");
		return NULL;
	}
	else if (nroBloque < 0)
		UT::LogError(UT::errSS << "Error al intentar agregar un nodo al archivo, identificador invalido.");

	int nroNodo = NroBloqueToNroNodo(nroBloque);
	RegControlNodo rcn;
	rcn.Nivel = nivel;

	Registro* regCtrl = GuardarRegistroControlNodo(rcn);

	if (!bl->insertarRegistroDeControl(regCtrl))
		UT::LogError(UT::errSS << "Error al intentar insertar registro de control de nodo.");

	delete regCtrl;

	if (nivel == 0)
	{
		BKDNodoHojaDisco* nodoHoja = new BKDNodoHojaDisco(this, nroNodo, this->m_capacidad_hoja, nivel);

		if (nodoHoja->EscribirEnBloque(bl))
			nodo = nodoHoja;
		else
			UT::LogError(UT::errSS << "Error al escribir nodo a Bloque. Nro de nodo: " << nroNodo);
	}
	else if (nivel > 0)
	{
		BKDNodoInternoDisco* nodoInterno = new BKDNodoInternoDisco(this, nroNodo, this->m_capacidad_interno, nivel);

		if (nodoInterno->EscribirEnBloque(bl))
			nodo = nodoInterno;
		else
			UT::LogError(UT::errSS << "Error al escribir nodo a Bloque. Nro de nodo: " << nroNodo);
	}

	if (!m_arch->ActualizarBloque(nroBloque, *bl))
		UT::LogError(UT::errSS << "Error al intentar actualizar bloque " << nroBloque);

	delete bl;
	return nodo;
}

bool BKDManagerDisco::GuardarNodo(BKDNodo* nodo)
{
	bool res = false;

	if (nodo == NULL)
	{
		UT::LogError(UT::errSS << "Error al intentar guardar nodo: el argumento es NULL.");
		return false;
	}

	//voy a disco a obtener el bloque indicado.
	int nroBloque = NroNodoToNumBloque(nodo->GetNumeroNodo());

	if (nroBloque < 0)
		UT::LogError(UT::errSS << "Error al intentar guardar nodo: identificador invalido.");

	Bloque* bl = m_arch->GetBloque(nroBloque);

	if (bl == NULL)
	{
		UT::LogError(UT::errSS << "Error: No se hallo el bloque " << nroBloque);
		return false;
	}

	RegControlNodo rcn;
	rcn.Nivel = nodo->GetNivel();
	Registro* regCtrl = GuardarRegistroControlNodo(rcn);

	if (!bl->actualizarRegistroDeControl(regCtrl))
	{
		UT::LogError(UT::errSS << "Error al intentar actualizar registro de control en bloque " << nroBloque);
		res = false;
	}

	delete regCtrl;

	if (nodo->GetNivel() == 0)
	{
		BKDNodoHojaDisco* nodoHoja = (BKDNodoHojaDisco*)nodo;

		if (nodoHoja->EscribirEnBloque(bl))
			res = true;
		else
			UT::LogError(UT::errSS << "Error al escribir nodo a Bloque. Nro de nodo: " << nodo->GetNumeroNodo());
	}
	else if (nodo->GetNivel() > 0)
	{
		BKDNodoInternoDisco* nodoInterno = (BKDNodoInternoDisco*)nodo;

		if (nodoInterno->EscribirEnBloque(bl))
			res = true;
		else
			UT::LogError(UT::errSS << "Error al escribir nodo a Bloque. Nro de nodo: " << nodo->GetNumeroNodo());
	}

	if (!m_arch->ActualizarBloque(nroBloque, *bl))
			UT::LogError(UT::errSS << "Error al intentar actualizar bloque " << nroBloque);

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
