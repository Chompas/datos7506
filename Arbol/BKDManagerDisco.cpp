/*
 * BKDManagerDisco.cpp
 *
 *  Created on: May 28, 2012
 *      Author: vdiego
 */

#include "BKDManagerDisco.h"
#include <iostream>

using namespace std;


//************ METODOS PRIVADOS ******************//

/*
BKDNodo* GetCopiaNodo(BKDNodo* nodo)
{
	if (nodo == NULL)
		return NULL;
	else if (nodo->EsNodoHoja())
		return new BKDNodoHoja((const BKDNodoHoja&)(*nodo));
	else
		return new BKDNodoInterno((const BKDNodoInterno&)(*nodo));
}
*/

//***********************************************//


BKDManagerDisco::BKDManagerDisco(string filePath, int tamanioBytesBloque)
{
	this->m_raiz_id = 1; //La raiz siempre es el nodo 1 de datos
	this->m_raiz = NULL;

	this->m_capacidad_hoja = tamanioBytesBloque; //** TODO: calcular capacidad segun tamanio del bloque
	this->m_capacidad_interno = tamanioBytesBloque; //** TODO: calcular capacidad segun tamanio del bloque

	this->m_file_path = filePath;
	this->m_tamBloque = tamanioBytesBloque;

	//** TODO: Validar file path no null, tamanio bloque > 20

	this->m_file.open(filePath.c_str(), fstream::binary | fstream::in | fstream::out); //** TODO: Abrir archivo y chequear que esta todo bien
}

BKDManagerDisco::~BKDManagerDisco()
{
	if (this->m_raiz != NULL)
		delete this->m_raiz;

	if (this->m_file != NULL && this->m_file.is_open())
	{
		this->m_file.flush();
		this->m_file.close();
	}
}

BKDNodo* BKDManagerDisco::GetNodoRaiz()
{
	return this->GetNodo(this->m_raiz_id);
}

BKDNodo* BKDManagerDisco::GetNodoPrimeraHoja()
{
	//devuelvo la primera hoja, o NULL si no existe
	return NULL; //TODO
}

BKDNodo* BKDManagerDisco::GetNodo(int nroNodo)
{
	//si nroNodo == this->idRaiz, devuelvo una copia del puntero a la raiz que guardo en memoria
	//sino, voy a disco a obtener el nodo indicado.
	//si el puntero a raiz es null, la voy a buscar por primera vez a disco

	return NULL; //**TODO
}

BKDNodo* BKDManagerDisco::AgregarNodo(int nivel)
{
	//** TODO
	return NULL;
}

bool BKDManagerDisco::GuardarNodo(BKDNodo* nodo)
{
	//** TODO
	return false;
}

bool BKDManagerDisco::BorrarNodo(int nroNodo)
{
	//** TODO
	return false;
}
