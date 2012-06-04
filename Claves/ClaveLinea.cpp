/*
 * ClaveLinea.cpp
 *
 *  Created on: Jun 3, 2012
 *      Author: chompas
 */

#include "ClaveLinea.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include "../Disco/IndicesManager.h"

using namespace std;


ClaveLinea::ClaveLinea(const int& linea)
	: ClaveInt(linea)
{ }

string ClaveLinea::ToString() const {

	string valorStr;
	IndicesManager *indiceLinea = new IndicesManager("Archivos/lineas.txt");
	valorStr = indiceLinea->obtenerValor(this->Valor);
	delete indiceLinea;

	stringstream ss;
	ss << valorStr;
	string result = ss.str();
	return result;
}


// -1: this < clave
//  0: this == clave
//  1: this > clave
int ClaveLinea::Comparar(const BKDClave& clave) const
{
	ClaveLinea* c = NULL;
	int retVal;

	try
	{
		c = (ClaveLinea*)(&clave);
	}
	catch(...)
	{
		std::cerr << "Error al Comparar claves: Argumento Inválido" << std::endl;
		return 1;
	}

	//Si el usuario ingreso un guion asigno 0 para saber que esta clave no se usa
	//Eso se usa unicamente para las busquedas
	string claveLineaStr;
	string claveLineaStrAComp;
	if (this->Valor == 0) {
		retVal = 0;
	} else {
		IndicesManager *indiceLinea = new IndicesManager("Archivos/lineas.txt");
		claveLineaStr = indiceLinea->obtenerValor(this->Valor);
		claveLineaStrAComp = indiceLinea->obtenerValor(c->Valor);
	}

	//Chequeo si la claveLineaStrAComp existia en el archivo
	if (claveLineaStrAComp == "0") {
		//No existia la clave, devuelvo -1 para llevar la busqueda a un nodo incorrecto
		retVal = -1;
	} else if (claveLineaStr.compare(claveLineaStrAComp) < 0)
		retVal = -1;
	else if (claveLineaStr.compare(claveLineaStrAComp) == 0)
		retVal = 0;
	else
		retVal = 1;

	return retVal;
}

