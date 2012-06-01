/*
 * BKDManagerDisco.cpp
 *
 *  Created on: May 28, 2012
 *      Author: vdiego
 */

#include "BKDManagerDisco.h"
#include "BKDNodoHojaDisco.h"
#include "BKDNodoInternoDisco.h"
#include <iostream>

using namespace std;

int MIN_TAMANIO_BLOQUE = 20;


//************ METODOS PRIVADOS ******************//


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

//***********************************************//


BKDManagerDisco::BKDManagerDisco(string filePath, int tamanioBytesBloque)
{
	this->m_raiz_id = 1; //La raiz siempre es el nodo 1 de datos
	this->m_raiz = NULL;

	this->m_capacidad_hoja = tamanioBytesBloque; //** TODO: calcular capacidad segun tamanio del bloque
	this->m_capacidad_interno = tamanioBytesBloque; //** TODO: calcular capacidad segun tamanio del bloque

	this->m_file_path = filePath;
	this->m_tamBloque = tamanioBytesBloque;



	if (filePath.empty())
		cerr << "Debe especificarse un archivo para el indice" << endl;

	if (tamanioBytesBloque < MIN_TAMANIO_BLOQUE)
		cerr << "Tamanio de Bloque insuficiente" << endl;

	this->m_file.open(filePath.c_str(), fstream::binary | fstream::in | fstream::out);

	//Cargo la raiz
	RefrescarRaiz();
}

BKDManagerDisco::BKDManagerDisco(std::string filePath)
{
	this->m_raiz_id = 1; //La raiz siempre es el nodo 1 de datos
	this->m_raiz = NULL;

	this->m_file_path = filePath;
	this->m_tamBloque = 0; //TODO: Leer de campo de control en el archivo

	this->m_capacidad_hoja = 0; //** TODO: calcular capacidad segun tamanio del bloque
	this->m_capacidad_interno = 0; //** TODO: calcular capacidad segun tamanio del bloque


	if (filePath.empty())
		cerr << "Debe especificarse un archivo para el indice" << endl;

	this->m_file.open(filePath.c_str(), fstream::binary | fstream::in | fstream::out);


	//**LEER CAMPOS CONTROL


	if (this->m_tamBloque < MIN_TAMANIO_BLOQUE)
		cerr << "Tamanio de Bloque insuficiente" << endl;

	//Cargo la raiz
	RefrescarRaiz();
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

	//Buscar bloque por nro de nodo
	//Cargar bloque en memoria
	//generar buffer e hidratar nodo desde el buffer
	//devolver nodo

	return NULL; //**TODO
}

BKDNodo* BKDManagerDisco::CrearNodoOffline(int nivel)
{
	//Nodo hoja o intermedio?
	if (nivel == 0)
		return new BKDNodoHojaDisco(this, -1, this->m_capacidad_hoja, nivel);
	else
		return new BKDNodoInternoDisco(this, -1, this->m_capacidad_interno, nivel);

}

BKDNodo* BKDManagerDisco::AgregarNodo(int nivel)
{
	//** TODO

	//Pedir nodo vacio
	//crear objeto nodo hoja/interno segun nivel, y asignarle el valor correspondiente de nro nodo
	//guardar los cambios hechos al archivo y devolver el nodo


	return NULL;
}

bool BKDManagerDisco::GuardarNodo(BKDNodo* nodo)
{
	//** TODO

	//Traer a memoria el bloque de archivo correspondiente al nodo (x nro nodo)
	//Generar buffer sobre el bloque y serializar el nodo sobre el buffer y bloque
	//guardar a disco el bloque y actualizar los campos de control



	if (nodo->GetNumeroNodo() == this->m_raiz_id)
			RefrescarRaiz();


	return false;
}

bool BKDManagerDisco::BorrarNodo(int nroNodo)
{
	//** TODO
	return false;
}
