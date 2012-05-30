/*
 * BKDNodoHoja.cpp
 *
 *  Created on: May 27, 2012
 *      Author: vdiego
 */

#include "BKDNodoHoja.h"
#include <iostream>

using namespace std;

typedef list<TRegistro>::iterator RegsIterator;




BKDNodoHoja::BKDNodoHoja(BKDManager* manager, int nroNodo, int capacidad, int nivel)
{
	this->m_capacidad = capacidad;
	this->m_nivel = nivel;
	this->m_manager = manager;
	this->m_nro_nodo = nroNodo;
	this->siguienteHoja = 0;
	this->registros.clear();
}

BKDNodoHoja::BKDNodoHoja(const BKDNodoHoja& ref)
{
	this->m_capacidad = ref.m_capacidad;
	this->m_nivel = ref.m_nivel;
	this->m_manager = ref.m_manager;
	this->m_nro_nodo = ref.m_nro_nodo;
	this->siguienteHoja = ref.siguienteHoja;
	this->registros.clear();
	this->registros.assign(ref.registros.begin(), ref.registros.end());
}

int BKDNodoHoja::GetSiguienteHoja()
{
	return this->siguienteHoja;
}

bool BKDNodoHoja::HayOverflow()
{
	return (this->registros.size() > this->m_capacidad);
}

bool BKDNodoHoja::HayUnderflow()
{
	//si soy hoja y raiz, no tengo un minimo de registros, siempre puedo eliminar.
	if (this->m_nivel == 0)
		return false;

	//sino, tengo que asegurar tener al menos capacidad / 2 registros.
	return (this->registros.size() < (this->m_capacidad / 2));
}

bool BKDNodoHoja::BuscarReg(const TClave& clave, TRegistro& registro)
{
	//recorro en orden de claves, si existe lleno el registro

	RegsIterator it = this->registros.begin();

	while(it != this->registros.end() && it->clave < clave)
		it++;

	//si llegue al final de la lista o corte porque habia una clave mayor
	if (it == this->registros.end() || it->clave != clave)
		return false;

	registro.clave = it->clave;
	registro.valor = it->valor;
	return true;
}


bool BKDNodoHoja::BuscarRango(const TClave& claveInicio, const TClave& claveFin, std::list<TRegistro>& resultado)
{
	//recorro desde el primer registro y mientras la clave sea menor o igual al hasta
	//si ademas es mayor o igual al desde, la agrego al resultado.

	RegsIterator it = this->registros.begin();

	while(it != this->registros.end() && it->clave <= claveFin)
	{
		if (it->clave >= claveInicio)
			resultado.push_back(*it);

		it++;
	}

	return true;
}


bool BKDNodoHoja::InsertarReg(const TRegistro& registro, bool& overflow)
{
	//soy hoja, asi que inserto en el orden correspondiente, y si me pase de la capacidad tiro overflow

	RegsIterator it = this->registros.begin();

	while(it != this->registros.end() && it->clave <= registro.clave)
		it++;

	this->registros.insert(it, registro);

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

bool BKDNodoHoja::ModificarReg(const TRegistro& registro)
{
	return false;
}

bool BKDNodoHoja::EliminarReg(const TClave& clave)
{
	return false;
}

//Resuelve un overflow en el nodo actual. Devuelve el nuevo hermano creado, y la clave a promover por referencia
//Ante un error, devuelve NULL como retorno
BKDNodo* BKDNodoHoja::ResolverOverflow(TClave& clavePromovida)
{
	//verifico que estoy en condicion de overflow
	if (!this->HayOverflow())
		return NULL;

	BKDNodoHoja* ret = NULL;
	ret = (BKDNodoHoja*)this->m_manager->AgregarNodo(this->m_nivel);

	if (ret != NULL)
	{
		ret->siguienteHoja = this->siguienteHoja;
		this->siguienteHoja = ret->m_nro_nodo;
		int aCopiar = this->m_capacidad / 2;
		RegsIterator ri = this->registros.end();
		TRegistro reg;

		for(int i = 0; i < aCopiar; i++)
		{

			ri--;
			reg.clave = ri->clave;
			reg.valor = ri->valor;
			ret->registros.push_front(reg);
			this->registros.erase(ri);
		}

		//Obtengo la clave del primero de los registros del nodo derecho (el nuevo)
		clavePromovida = reg.clave;
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
	nodo->siguienteHoja = this->siguienteHoja;
	nodo->registros.clear();

	for (RegsIterator ri = this->registros.begin(); ri != this->registros.end(); ri++)
	{
		TRegistro reg;
		reg.clave = ri->clave;
		reg.valor = ri->valor;
		nodo->registros.push_back(reg);
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

	for (RegsIterator ri = this->registros.begin(); ri != this->registros.end(); ri++)
	{
		std::cout << " [";
		std::cout << "('";
		std::cout << ri->clave;
		std::cout << "') , ";
		std::cout << "(";
		std::cout << ri->valor;
		std::cout << ")";
		std::cout << "] ";
	}

	std::cout << " |<" << this->siguienteHoja << ">|";

}

