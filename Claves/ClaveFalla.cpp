/*
 * ClaveFalla.cpp
 *
 *  Created on: Jun 3, 2012
 *      Author: chompas
 */

#include "ClaveFalla.h"
#include <iostream>
#include <cstring>
#include "../Disco/IndicesManager.h"

using namespace std;

ClaveFalla::ClaveFalla(const int& falla)
	: ClaveInt(falla)
{ }

// -1: this < clave
//  0: this == clave
//  1: this > clave
int ClaveFalla::Comparar(const BKDClave& clave) const
{
	ClaveFalla* c = NULL;
	int retVal;

	try
	{
		c = (ClaveFalla*)(&clave);
	}
	catch(...)
	{
		std::cerr << "Error al Comparar claves: Argumento Inválido" << std::endl;
		return 1;
	}

	//Si el usuario ingreso un guion asigno 0 para saber que esta clave no se usa
	//Eso se usa unicamente para las busquedas
	string claveFallaStr;
	string claveFallaStrAComp;
	if (this->Valor == 0) {
		retVal = 0;
	} else {
		IndicesManager *indiceFallas = new IndicesManager("Archivos/fallas.txt");
		claveFallaStr = indiceFallas->obtenerValor(this->Valor);
		claveFallaStrAComp = indiceFallas->obtenerValor(c->Valor);
	}

	//Chequeo si la lineaClaveAComp existia en el archivo
	if (claveFallaStrAComp == "0") {
		//No existia la clave, devuelvo -1 para llevar la busqueda a un nodo incorrecto
		retVal = -1;
	} else if (claveFallaStr.compare(claveFallaStrAComp) < 0)
		retVal = -1;
	else if (claveFallaStr.compare(claveFallaStrAComp) == 0)
		retVal = 0;
	else
		retVal = 1;

	return retVal;
}
