/*
 * BKDArbol.cpp
 *
 *  Created on: May 27, 2012
 *      Author: vdiego
 */

#include "BKDArbol.h"
#include "BKDNodoInterno.h"
#include "BKDManagerMemoria.h"
#include "BKDManagerDisco.h"

#include <iostream>

using namespace std;

BKDArbol::~BKDArbol()
{
	delete this->m_manager;
}


//capacidad en numero de registros y numero de claves, respectivamente
BKDArbol* BKDArbol::CrearEnMemoria(const int capacidadNodoHoja, const int capacidadNodoInterno)
{
	BKDArbol* arbol = new BKDArbol();
	arbol->m_manager = new BKDManagerMemoria(capacidadNodoHoja, capacidadNodoInterno);

	return arbol;
}

//Para rendimiento optimo utilizar bloques de 512 * 2^N Bytes
BKDArbol* BKDArbol::CrearEnDisco(const std::string& filePath, const int tamanioBloqueBytes)
{
	return NULL; //** TODO
}

BKDArbol* BKDArbol::AbrirDeDisco(const std::string& filePath)
{
	return NULL; //** TODO
}







bool BKDArbol::BuscarRegistro(const string& clave, TRegistro& registro)
{
	BKDNodo* raiz = this->m_manager->GetNodoRaiz();
	bool res = false;

	if (raiz != NULL)
	{
		res = raiz->BuscarReg(clave, registro);
		delete raiz;
	}

	return res;
}


bool BKDArbol::BuscarPorRango(const TClave& claveInicio, const TClave& claveFin, std::list<TRegistro>& resultado)
{
	resultado.clear();

	if (claveFin < claveInicio)
	{
		cerr << "Error: La clave de inicio debe ser menor o igual a la de fin" << endl;
		return false;
	}

	BKDNodo* raiz = this->m_manager->GetNodoRaiz();
	bool res = false;

	if (raiz != NULL)
	{
		res = raiz->BuscarRango(claveInicio, claveFin, resultado);
		delete raiz;
	}

	return res;
}


bool BKDArbol::InsertarRegistro(const TRegistro& registro)
{
	BKDNodo* raiz = this->m_manager->GetNodoRaiz();

	//Si aun no existe la raiz, tengo que crearla
	if (raiz == NULL)
		raiz = this->m_manager->AgregarNodo(0);

	//Si fallo la insercion...
	if (raiz == NULL)
	{
		cerr << "Error al intentar crear nodo Raiz" << endl;
		return false;
	}

	bool overflow = false;
	bool ret = true;

	ret = raiz->InsertarReg(registro, overflow);

	if (overflow)
	{
		//Si hay overflow en la raiz, tengo que crear un nuevo nivel en el arbol

		BKDNodo* reemplazo = this->m_manager->AgregarNodo(raiz->GetNivel());

		TClave clavePromovida;

		BKDNodo* hermanoDer = raiz->ResolverOverflow(clavePromovida);
		raiz->ClonarNodo(reemplazo, false);

		BKDNodoInterno* nuevaRaiz = BKDNodoInterno::PromoverRaiz(this->m_manager, raiz, clavePromovida,
				reemplazo->GetNumeroNodo(), hermanoDer->GetNumeroNodo());

		if (!this->m_manager->GuardarNodo(reemplazo))
			cerr << "Error al intentar guardar nuevo hijo izquierdo: " << reemplazo->GetNumeroNodo() << endl;
		if (!this->m_manager->GuardarNodo(hermanoDer))
			cerr << "Error al intentar guardar nuevo hijo derecho: " << hermanoDer->GetNumeroNodo() << endl;
		if (!this->m_manager->GuardarNodo(nuevaRaiz))
			cerr << "Error al intentar guardar nueva raiz: " << nuevaRaiz->GetNumeroNodo() << endl;

		delete reemplazo;
		delete hermanoDer;
		delete nuevaRaiz;

		ret = true;
	}

	delete raiz;

	if (!ret)
		cerr << "Error al intentar insertar registro con clave = " << registro.clave << endl;

	return ret;
}

bool BKDArbol::ModificarRegistro(const TRegistro& registro)
{
	return false;
}

bool BKDArbol::EliminarRegistro(const string& clave)
{
	return false;
}



void BKDArbol::DebugPrint(int nivel)
{
	std::cout << endl;

	BKDNodo* raiz = this->m_manager->GetNodoRaiz();

	if (raiz != NULL)
	{
		raiz->DebugPrint(nivel);
		delete raiz;
	}
	else
		std::cout << "ARBOL VACIO" << endl;

	std::cout << endl;
}

void BKDArbol::DebugPrintAll()
{
	std::cout << endl;

	BKDNodo* raiz = this->m_manager->GetNodoRaiz();

	if (raiz == NULL)
	{
		std::cout << "ARBOL VACIO" << endl;
		return;
	}

	for (int n = raiz->GetNivel(); n >= 0; n--)
	{
		this->DebugPrint(n);
	}

	delete raiz;

}



