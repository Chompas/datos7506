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
	RegistroDeLongitudVariable* reg = new RegistroDeLongitudVariable((char*)(&regControl.Nivel), sizeof(int));
	return reg;
}

int GetTamanioRegistroControlNodo()
{
	int aux = 0;
	RegistroDeLongitudVariable* reg = new RegistroDeLongitudVariable((char*)&aux, sizeof(int));
	aux = reg->getLongitud();
	delete reg;
	return aux;
}

void BKDManagerDisco::ActualizarCapacidadNodos(Bloque* bloque)
{
	if (bloque == NULL)
	{
		UT::LogError(UT::errSS << "Error al intentar Actualizar Capacidad de Nodos: Bloque NULL");
		this->m_capacidad_hoja = -1;
		this->m_capacidad_interno = -1;
		return;
	}

	if (this->m_capacidad_hoja == -1 || this->m_capacidad_interno == -1)
	{
		int capRegistros = this->m_arch->GetCapacidadMaximaParaRegistros(bloque);

		//a la capacidad para registros le resto lo que ocupa el registro de control
		RegControlNodo rcn;
		rcn.Nivel = -1;
		Registro* aux = GuardarRegistroControlNodo(rcn);
		capRegistros -= this->m_arch->CalcularTamanioFinalRegistro(aux);
		delete aux;

		UT::LogDebug(UT::dbgSS << "Actualizando Capacidad de Nodos. Nueva capacidad: " << capRegistros);

		this->m_capacidad_hoja = capRegistros;
		this->m_capacidad_interno = capRegistros;
	}
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
	Utils::LogDebug(Utils::dbgSS << "Cargando nodo raiz desde el archivo... ");

	if (this->m_raiz != NULL)
	{
		Utils::LogDebug(Utils::dbgSS << "Borrando raiz en memoria... ");
		delete this->m_raiz;
		this->m_raiz = NULL;
	}

	Utils::LogDebug(Utils::dbgSS << "Buscando raiz en disco... ");
	this->m_raiz = this->GetNodo(this->m_raiz_id);

	if (this->m_raiz == NULL)
		Utils::LogError(Utils::errSS << "Error al intentar cargar raiz desde disco");
	else
		Utils::LogDebug(Utils::dbgSS << "Raiz cargada OK.");
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
	Utils::LogDebug(Utils::dbgSS << "Creando BKDManagerDisco..." << endl
								 << "\t\t Archivo: " << filePath << endl
								 << "\t\t Tamanio Bloque: " << tamanioBytesBloque);

	this->m_raiz_id = 1; //La raiz siempre es el nodo 1 de datos
	this->m_raiz = NULL;
	this->m_capacidad_hoja = -1;
	this->m_capacidad_interno = -1;


	if (filePath.empty())
	{
		UT::LogError(UT::errSS << "Debe especificarse un archivo para el indice.");
		//return; //throw ex?
		throw "Nombre de archivo Invalido.";
	}

	if (tamanioBytesBloque < ArchivoBloques::GetTamanioMinimoBloque())
	{
		UT::LogError(UT::errSS << "Tamanio de Bloque insuficiente");
		//return; //throw ex?
		throw "Tamanio de Bloque insuficiente.";
	}

	if (instanciadorRegistros == NULL)
	{
		UT::LogError(UT::errSS << "Debe especificarse un instanciador de registros");
		//return; //throw ex?
		throw "No se encontro instanciador de registros.";
	}

	this->m_instanciador = instanciadorRegistros;

	//creo el archivo
	this->m_arch = new ArchivoBloques(filePath, tamanioBytesBloque);

	if (!this->m_arch->EstadoOK())
	{
		UT::LogError(UT::errSS << "Ocurrio un error al crear el archivo de datos.");
		delete this->m_instanciador;
		delete this->m_arch;
		//return;
		throw "Error al crear archivo.";
	}

	Utils::LogDebug(Utils::dbgSS << "Creando Nodo Raiz...");
	//creo el nodo raiz (ya lo guarda en disco)
	this->m_raiz = this->AgregarNodo(0);

	//Recargo la raiz a memoria
	RefrescarRaiz();
}

BKDManagerDisco::BKDManagerDisco(const std::string& filePath, BKDInstanciador* instanciadorRegistros)
{
	Utils::LogDebug(Utils::dbgSS << "Creando BKDManagerDisco..." << endl
								 << "\t\t Archivo: " << filePath << endl
								 << "\t\t Tamanio Bloque: Desconocido");

	this->m_raiz_id = 1; //La raiz siempre es el nodo 1 de datos
	this->m_raiz = NULL;
	this->m_capacidad_hoja = -1;
	this->m_capacidad_interno = -1;

	if (filePath.empty())
	{
		UT::LogError(UT::errSS << "Debe especificarse un archivo para el indice");
		//return; //throw ex?
		throw "Nombre de archivo invalido.";
	}

	if (instanciadorRegistros == NULL)
	{
		UT::LogError(UT::errSS << "Debe especificarse un instanciador de registros");
		//return; //throw ex?
		throw "No se encontro instanciador de registros.";
	}

	this->m_instanciador = instanciadorRegistros;

	this->m_arch = new ArchivoBloques(filePath);

	if (!this->m_arch->EstadoOK())
	{
		UT::LogError(UT::errSS << "Ocurrio un error al abrir el archivo de datos.");
		delete this->m_instanciador;
		delete this->m_arch;
		//return;
		throw "Error al abrir archivo.";
	}

	Utils::LogDebug(Utils::dbgSS << "Archivo de arbol leido. Capacidad de nodos: " << this->m_capacidad_hoja);

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
	Utils::LogDebug(Utils::dbgSS << "Buscando en disco Nodo nro: " << nroNodo);

	//voy a disco a obtener el nodo indicado.
	int nroBloque = NroNodoToNumBloque(nroNodo);
	Bloque* bl = m_arch->GetBloque(nroBloque);

	if (bl == NULL)
	{
		UT::LogError(UT::errSS << "Error: No se hallo el nodo " << nroNodo);
		return NULL;
	}

	//Actualizo, si es necesario, la capacidad de los nodos a partir de la informacion de bloque
	this->ActualizarCapacidadNodos(bl);

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

BKDClave* BKDManagerDisco::InstanciarClave()
{
	return this->m_instanciador->InstanciarClave();
}

int BKDManagerDisco::CalcularEspacioAOcupar(Registro* registro)
{
	return this->m_arch->CalcularTamanioFinalRegistro(registro);
}


BKDNodo* BKDManagerDisco::AgregarNodo(int nivel)
{
	Utils::LogDebug(Utils::dbgSS << "Agregando Nodo... (Nivel: " << nivel << ")");

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
	{
		UT::LogError(UT::errSS << "Error al intentar agregar un nodo al archivo, identificador invalido.");
		delete bl;
		return NULL;
	}

	int nroNodo = NroBloqueToNroNodo(nroBloque);
	RegControlNodo rcn;
	rcn.Nivel = nivel;

	Utils::LogDebug(Utils::dbgSS << "Nuevo numero de nodo: " << nroNodo << " (Bloque: " << nroBloque << ")" );

	Utils::LogDebug(Utils::dbgSS << "Generando registro de control con informacion de nivel del nodo...");
	Registro* regCtrl = GuardarRegistroControlNodo(rcn);

	if (regCtrl == NULL)
	{
		UT::LogError(UT::errSS << "Error al generar registro de control del nodo.");
		delete bl;
		return NULL;
	}

	Utils::LogDebug(Utils::dbgSS << "Informacion de control generada OK. Intentando insertar registro de control al bloque...");

	if (!bl->insertarRegistroDeControl(regCtrl))
	{
		UT::LogError(UT::errSS << "Error al intentar insertar registro de control de nodo.");
		delete regCtrl;
		delete bl;
		return NULL;
	}

	Utils::LogDebug(Utils::dbgSS << "Se inserto correctamente el registro de control.");

	delete regCtrl;

	//Actualizo, si es necesario, la capacidad de los nodos a partir de la informacion de bloque
	this->ActualizarCapacidadNodos(bl);

	if (nivel == 0)
	{
		Utils::LogDebug(Utils::dbgSS << "Creando nodo hoja..." << endl
									 << "\t\t Nivel: " << nivel << endl
									 << "\t\t Capacidad: " << this->m_capacidad_hoja << endl
									 << "\t\t Nro: " << nroNodo);

		BKDNodoHojaDisco* nodoHoja = new BKDNodoHojaDisco(this, nroNodo, this->m_capacidad_hoja, nivel);

		if (nodoHoja->EscribirEnBloque(bl))
			nodo = nodoHoja;
		else
			UT::LogError(UT::errSS << "Error al escribir nodo a Bloque. Nro de nodo: " << nroNodo);
	}
	else if (nivel > 0)
	{
		Utils::LogDebug(Utils::dbgSS << "Creando nodo interno..." << endl
									 << "\t\t Nivel: " << nivel << endl
									 << "\t\t Capacidad: " << this->m_capacidad_interno << endl
									 << "\t\t Nro: " << nroNodo);

		BKDNodoInternoDisco* nodoInterno = new BKDNodoInternoDisco(this, nroNodo, this->m_capacidad_interno, nivel);

		if (nodoInterno->EscribirEnBloque(bl))
			nodo = nodoInterno;
		else
			UT::LogError(UT::errSS << "Error al escribir nodo a Bloque. Nro de nodo: " << nroNodo);
	}

	Utils::LogDebug(Utils::dbgSS << "Intentando grabar bloque a disco...");

	if (!m_arch->ActualizarBloque(nroBloque, *bl))
		UT::LogError(UT::errSS << "Error al intentar actualizar bloque " << nroBloque);
	else
		Utils::LogDebug(Utils::dbgSS << "Bloque grabado OK. (Bloque nro: " << nroBloque << ")");

	delete bl;
	return nodo;
}

bool BKDManagerDisco::GuardarNodo(BKDNodo* nodo)
{
	if (nodo == NULL)
	{
		UT::LogError(UT::errSS << "Error al intentar guardar nodo: el argumento es NULL.");
		return false;
	}

	//voy a disco a obtener el bloque indicado.
	int nroBloque = NroNodoToNumBloque(nodo->GetNumeroNodo());

	if (nroBloque < 0)
	{
		UT::LogError(UT::errSS << "Error al intentar guardar nodo: identificador invalido.");
		return false;
	}

	Bloque* bl = m_arch->GetBloque(nroBloque);

	if (bl == NULL)
	{
		UT::LogError(UT::errSS << "Error: No se hallo el bloque " << nroBloque);
		return false;
	}

	//lo vacio y lo piso con los nuevos datos
	bl->vaciar();

	RegControlNodo rcn;
	rcn.Nivel = nodo->GetNivel();
	Registro* regCtrl = GuardarRegistroControlNodo(rcn);

	if (regCtrl == NULL)
	{
		UT::LogError(UT::errSS << "Error al intentar actualizar registro de control en bloque " << nroBloque);
		delete bl;
		return false;
	}

	if (!bl->insertarRegistroDeControl(regCtrl))
	{
		UT::LogError(UT::errSS << "Error al intentar actualizar registro de control en bloque " << nroBloque);
		delete bl;
		delete regCtrl;
		return false;
	}

	delete regCtrl;

	if (nodo->GetNivel() == 0)
	{
		BKDNodoHojaDisco* nodoHoja = (BKDNodoHojaDisco*)nodo;

		if (!nodoHoja->EscribirEnBloque(bl))
		{
			UT::LogError(UT::errSS << "Error al escribir nodo a Bloque. Nro de nodo: " << nodo->GetNumeroNodo());
			delete bl;
			return false;
		}
	}
	else if (nodo->GetNivel() > 0)
	{
		BKDNodoInternoDisco* nodoInterno = (BKDNodoInternoDisco*)nodo;

		if (!nodoInterno->EscribirEnBloque(bl))
		{
			UT::LogError(UT::errSS << "Error al escribir nodo a Bloque. Nro de nodo: " << nodo->GetNumeroNodo());
			delete bl;
			return false;
		}
	}

	if (!m_arch->ActualizarBloque(nroBloque, *bl))
	{
		UT::LogError(UT::errSS << "Error al intentar actualizar bloque " << nroBloque);
		delete bl;
		return false;
	}

	delete bl;

	if (nodo->GetNumeroNodo() == this->m_raiz_id)
		RefrescarRaiz();

	return true;
}

bool BKDManagerDisco::BorrarNodo(int nroNodo)
{
	//** TODO

	return false;
}
