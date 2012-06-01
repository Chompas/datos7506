/*
 * BKDManagerMemoria.cpp
 *
 *  Created on: May 28, 2012
 *      Author: vdiego
 */

#include "BKDManagerMemoria.h"
#include "BKDNodoHoja.h"
#include "BKDNodoInterno.h"
#include <iostream>

using namespace std;

typedef map<int,BKDNodo*>::iterator MapIterator;
typedef pair<int,BKDNodo*> MapValue;


//************ METODOS PRIVADOS ******************//

BKDNodo* GetCopiaNodo(BKDNodo* nodo)
{
	if (nodo == NULL)
		return NULL;
	else if (nodo->EsNodoHoja())
		return new BKDNodoHoja((const BKDNodoHoja&)(*nodo));
	else
		return new BKDNodoInterno((const BKDNodoInterno&)(*nodo));
}

//***********************************************//



BKDManagerMemoria::BKDManagerMemoria(int capacidadNodoHoja, int capacidadNodoInterno)
{
	this->m_raiz_id = 1; //La raiz siempre es el nodo 1.

	this->m_capacidad_hoja = capacidadNodoHoja > 0 ? capacidadNodoHoja : 1;
	this->m_capacidad_interno = capacidadNodoInterno > 0 ? capacidadNodoInterno : 1;

	this->m_nodes.clear();
}

BKDManagerMemoria::~BKDManagerMemoria()
{
	if (this->m_nodes.size() > 0)
	{
		//recorrer los m_nodos y deletearlos
		for (MapIterator it = this->m_nodes.begin();
				it != this->m_nodes.end(); it++)
		{
			delete it->second;
		}

		this->m_nodes.clear();
	}
}

BKDNodo* BKDManagerMemoria::GetNodoRaiz()
{
	return this->GetNodo(this->m_raiz_id);
}

BKDNodo* BKDManagerMemoria::GetNodoPrimeraHoja()
{
	//devuelvo la primera hoja, o NULL si no existe
	return NULL; //TODO
}

BKDNodo* BKDManagerMemoria::GetNodo(int nroNodo)
{
	MapIterator it = this->m_nodes.find(nroNodo);
	if (it == this->m_nodes.end())
		return NULL; //No existe la clave
	else
		return GetCopiaNodo(it->second);
}

BKDNodo* BKDManagerMemoria::CrearNodoOffline(int nivel)
{
	//Nodo hoja o intermedio?
	if (nivel == 0)
		return new BKDNodoHoja(this, -1, this->m_capacidad_hoja, nivel);
	else
		return new BKDNodoInterno(this, -1, this->m_capacidad_interno, nivel);

}

BKDNodo* BKDManagerMemoria::AgregarNodo(int nivel)
{
	MapIterator it = (this->m_nodes.end()); //me voy al final
	int nroNodo = 1; //el primer nodo es el 1

	//busco el mayor nroNodo y le sumo 1
	if (this->m_nodes.size() > 0)
	{
		it--;
		nroNodo = it->first + 1;
	}

	BKDNodo* nuevoNodo = NULL;

	//si nivel == 0 => es hoja, sino interno
	if(nivel == 0)
		nuevoNodo = new BKDNodoHoja(this, nroNodo, this->m_capacidad_hoja, nivel);
	else
		nuevoNodo = new BKDNodoInterno(this, nroNodo, this->m_capacidad_interno, nivel);

	MapIterator res = this->m_nodes.insert(it, MapValue(nroNodo, nuevoNodo));

	//si no lo pude insertar, borro la memoria y devuelvo NULL
	if (res != this->m_nodes.end() && res->first != nroNodo)
	{
		delete nuevoNodo;
		return NULL;
	}

	return GetCopiaNodo(nuevoNodo);
}

bool BKDManagerMemoria::GuardarNodo(BKDNodo* nodo)
{
	if (nodo == NULL)
	{
		cerr << "Error: no se puede guardar un nodo NULL" << endl;
		return false;
	}

	//Es un nodo "basura", no lo guardo
	if (nodo->GetNumeroNodo() <= 0)
	{
		cerr << "Error: identificador de nodo invalido" << endl;
		return false;
	}

	//Me fijo si se modifico la referencia y la actualizo
	MapIterator it = this->m_nodes.find(nodo->GetNumeroNodo());
	if (it == this->m_nodes.end())
	{
		cerr << "Error: no se encuentra el nodo con identificador = " << nodo->GetNumeroNodo() << ". Debe usar el metodo 'AgregarNodo()' para crear nuevos nodos." << endl;
		return false; //No existe la clave, debe insertarlo primero
	}
	else
	{
		BKDNodo* nodoAlmacenado = this->m_nodes[nodo->GetNumeroNodo()];

		if (nodoAlmacenado != NULL)
		{
			delete nodoAlmacenado;
			nodoAlmacenado = NULL;
			this->m_nodes[nodo->GetNumeroNodo()] = GetCopiaNodo(nodo);
		}

		return true;
	}
}

bool BKDManagerMemoria::BorrarNodo(int nroNodo)
{
	MapIterator it = this->m_nodes.find(nroNodo);
	if (it == this->m_nodes.end())
		return false; //No existe la clave
	else
	{
		delete it->second;
		this->m_nodes.erase(it);
		return true;
	}
}

