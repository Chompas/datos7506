/*
 * BKDNodoHoja.cpp
 *
 *  Created on: May 27, 2012
 *      Author: vdiego
 */

#include "BKDNodoHoja.h"
#include <iostream>

using namespace std;

typedef list<BKDRegistro*>::iterator RegsIterator;


BKDNodoHoja::~BKDNodoHoja()
{
	if (this->m_registros.size() > 0)
	{
		//recorrer los registros y deletearlos
		for (RegsIterator it = this->m_registros.begin(); it != this->m_registros.end(); it++)
		{
			delete *it;
		}

		this->m_registros.clear();
	}
}

BKDNodoHoja::BKDNodoHoja(BKDManager* manager, int nroNodo, int capacidad, int nivel)
{
	this->m_capacidad = capacidad;
	this->m_nivel = nivel;
	this->m_manager = manager;
	this->m_nro_nodo = nroNodo;
	this->m_siguienteHoja = 0;
	this->m_registros.clear();
}

BKDNodoHoja::BKDNodoHoja(const BKDNodoHoja& ref)
{
	this->m_capacidad = ref.m_capacidad;
	this->m_nivel = ref.m_nivel;
	this->m_manager = ref.m_manager;
	this->m_nro_nodo = ref.m_nro_nodo;
	this->m_siguienteHoja = ref.m_siguienteHoja;
	this->m_registros.clear();

	for (std::list<BKDRegistro*>::const_iterator i = ref.m_registros.begin(); i != ref.m_registros.end(); i++)
	{
		this->m_registros.push_back((*i)->Clonar());
	}

}

int BKDNodoHoja::GetSiguienteHoja()
{
	return this->m_siguienteHoja;
}

bool BKDNodoHoja::HayOverflow()
{
	return (this->m_registros.size() > this->m_capacidad);
}

bool BKDNodoHoja::HayUnderflow()
{
	//si soy hoja y raiz, no tengo un minimo de registros, siempre puedo eliminar.
	if (this->m_nivel == 0)
		return false;

	//sino, tengo que asegurar tener al menos capacidad / 2 registros.
	return (this->m_registros.size() < (this->m_capacidad / 2));
}

bool BKDNodoHoja::BuscarReg(const BKDClave& clave, BKDRegistro** registro)
{
	//recorro en orden de claves, si existe lleno el registro

	RegsIterator it = this->m_registros.begin();
	BKDClave* itClave = NULL;

	while(it != this->m_registros.end())
	{
		itClave = (*it)->GetClave();

		if (itClave->Comparar(clave) != -1)
			break;
		else
		{
			delete itClave;
			itClave = NULL;
		}

		it++;
	}

	//si llegue al final de la lista o corte porque habia una clave mayor
	if (it == this->m_registros.end() || itClave->Comparar(clave) != 0)
	{
		if (itClave != NULL)
		{
			delete itClave;
			itClave = NULL;
		}

		return false;
	}

	if (itClave != NULL)
	{
		delete itClave;
		itClave = NULL;
	}

	(*registro) = (*it)->Clonar();

	return true;
}


bool BKDNodoHoja::BuscarRango(const BKDClave& claveInicio, const BKDClave& claveFin, std::list<BKDRegistro*>& resultado)
{
	//recorro desde el primer registro y mientras la clave sea menor o igual al hasta
	//si ademas es mayor o igual al desde, la agrego al resultado.

	RegsIterator it = this->m_registros.begin();
	BKDClave* itClave = NULL;

	while(it != this->m_registros.end())
	{
		itClave = (*it)->GetClave();
		if (itClave->Comparar(claveFin) == 1)
			break;


		if (itClave->Comparar(claveInicio) != -1)
			resultado.push_back(((*it)->Clonar()));

		delete itClave;
		itClave = NULL;
		it++;
	}

	//Si corto porque encontro una clave menor, libero la memoria de la ultima clave
	if (it != this->m_registros.end() && itClave != NULL)
		delete itClave;

	return true;
}


bool BKDNodoHoja::InsertarReg(const BKDRegistro& registro, bool& overflow)
{
	//soy hoja, asi que inserto en el orden correspondiente, y si me pase de la capacidad tiro overflow

	RegsIterator it = this->m_registros.begin();
	BKDClave* regClave = registro.GetClave();
	BKDClave* itClave = NULL;

	while(it != this->m_registros.end())
	{
		itClave = (*it)->GetClave();

		if (itClave->Comparar(*regClave) == 1)
			break;

		delete itClave;
		itClave = NULL;
		it++;
	}

	//Si corto por clave, la libero
	if (it != this->m_registros.end() && itClave != NULL)
		delete itClave;

	delete regClave;

	this->m_registros.insert(it, registro.Clonar());

	overflow = this->HayOverflow();

	//Solo me guardo a mi mismo en el caso de que no este en overflow, sino me guarda papi :)
	if (!overflow)
		if(!this->m_manager->GuardarNodo(this))
		{
			cerr << "Error al intentar guardar nodo hoja: " << this->m_nro_nodo << endl;
			return false;
		}

	return !overflow;
}

bool BKDNodoHoja::ModificarReg(const BKDRegistro& registro)
{
	return false;
}

bool BKDNodoHoja::EliminarReg(const BKDClave& clave)
{
	return false;
}

//Resuelve un overflow en el nodo actual. Devuelve el nuevo hermano creado, y la clave a promover por referencia
//Ante un error, devuelve NULL como retorno
BKDNodo* BKDNodoHoja::ResolverOverflow(BKDClave** clavePromovida)
{
	//verifico que estoy en condicion de overflow
	if (!this->HayOverflow())
		return NULL;

	BKDNodoHoja* ret = NULL;
	ret = (BKDNodoHoja*)this->m_manager->AgregarNodo(this->m_nivel);

	if (ret != NULL)
	{
		ret->m_siguienteHoja = this->m_siguienteHoja;
		this->m_siguienteHoja = ret->m_nro_nodo;
		int aCopiar = this->m_capacidad / 2;
		RegsIterator ri = this->m_registros.end();

		for(int i = 0; i < aCopiar; i++)
		{
			ri--;
			ret->m_registros.push_front(*ri);
			this->m_registros.erase(ri);
		}

		//Obtengo la clave del primero de los registros del nodo derecho (el nuevo)
		(*clavePromovida) = (*(ret->m_registros.begin()))->GetClave();
	}

	return ret;
}

bool BKDNodoHoja::ClonarNodo(BKDNodo* nodoHoja, bool clonarNroNodo)
{
	if (nodoHoja == NULL)
		return false;

	if (!nodoHoja->EsNodoHoja())
		return false; //no es un nodo hoja

	BKDNodoHoja* nodo = (BKDNodoHoja*)nodoHoja;

	nodo->m_capacidad = this->m_capacidad;
	nodo->m_manager = this->m_manager;
	nodo->m_nivel = this->m_nivel;
	if (clonarNroNodo)
		nodo->m_nro_nodo = this->m_nro_nodo;
	nodo->m_siguienteHoja = this->m_siguienteHoja;

	if (nodo->m_registros.size() > 0)
	{
		//recorrer los registros y deletearlos
		for (RegsIterator it = nodo->m_registros.begin(); it != nodo->m_registros.end(); it++)
		{
			delete *it;
		}

		nodo->m_registros.clear();
	}

	for (RegsIterator ri = this->m_registros.begin(); ri != this->m_registros.end(); ri++)
	{
		nodo->m_registros.push_back((*ri)->Clonar());
	}

	return true;
}



//PARA DEBUG
void BKDNodoHoja::DebugPrint(int nivel)
{
	// si estoy en el nivel solicitado me imprimo
	if (this->m_nivel != nivel)
		return;

	std::cout << "|" << this->m_nro_nodo << "| ";

	for (RegsIterator ri = this->m_registros.begin(); ri != this->m_registros.end(); ri++)
	{
		std::cout << (*ri)->ToString();
	}

	std::cout << " |<" << this->m_siguienteHoja << ">|";
}

