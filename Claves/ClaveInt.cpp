/*
 * ClaveInt.cpp
 *
 *  Created on: Jun 1, 2012
 *      Author: vdiego
 */

#include "ClaveInt.h"
#include <iostream>
#include <cstring>
#include <sstream>


ClaveInt::ClaveInt(int valor)
{
	this->Valor = valor;
}

ClaveInt::ClaveInt(const ClaveInt& clave)
{
	this->Valor = clave.Valor;
}

ClaveInt::~ClaveInt() { }

// -1: this < clave
//  0: this == clave
//  1: this > clave
int ClaveInt::Comparar(const BKDClave& clave) const
{
	ClaveInt* c = NULL;

	try
	{
		c = (ClaveInt*)(&clave);
	}
	catch(...)
	{
		std::cerr << "Error al Comparar claves: Argumento Inválido" << std::endl;
		return 1;
	}

	if (this->Valor < c->Valor)
		return -1;
	else if (this->Valor == c->Valor)
		return 0;
	else
		return 1;
}

BKDClave* ClaveInt::Clonar() const
{
	return new ClaveInt(this->Valor);
}

std::string ClaveInt::ToString() const
{
	std::stringstream ss;
	ss << this->Valor;
	std::string result = ss.str();
	return result;
}


// Implementacion de Serializable

int ClaveInt::getLongitud()
{
	return sizeof(int);
}

int ClaveInt::serializar (Buffer* buffer, int posicion){
	int tam = sizeof(int);
	char* stream = new char[tam];
	char* ptr = stream;

	memcpy(ptr, &(this->Valor), tam);

	buffer->setStream(stream, tam);
	delete []stream;

	return 0;
}

int ClaveInt::hidratar (Buffer* buffer, int posicion){
	int tam;
	char* stream = buffer->getStream(tam);
	char* ptr = stream;

	if (tam != sizeof(int))
		std::cerr << "Error al hidratar ClaveInt, tamaño de dato inválido" << std::endl;

	memcpy(&(this->Valor), ptr, sizeof(int));
	ptr += sizeof(int);

	delete []stream;

	return 0;
}



