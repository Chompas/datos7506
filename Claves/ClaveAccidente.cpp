/*
 * ClaveAccidente.cpp
 *
 *  Created on: Jun 3, 2012
 *      Author: chompas
 */

#include "ClaveAccidente.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include "../Disco/IndicesManager.h"

using namespace std;


ClaveAccidente::ClaveAccidente(const int& accidente)
	: ClaveInt(accidente)
{ }

	string ClaveAccidente::ToString() const {

	string valorStr;
	IndicesManager *indiceLinea = new IndicesManager("Archivos/accidentes.txt");
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
int ClaveAccidente::Comparar(const BKDClave& clave) const
{
	ClaveAccidente* c = NULL;
	int retVal;

	try
	{
		c = (ClaveAccidente*)(&clave);
	}
	catch(...)
	{
		std::cerr << "Error al Comparar claves: Argumento Inválido" << std::endl;
		return 1;
	}

	//Si el usuario ingreso un guion asigno 0 para saber que esta clave no se usa
	//Eso se usa unicamente para las busquedas
	string claveAccidenteStr;
	string claveAccidenteStrAComp;
	if (c->Valor == 0) {
		retVal = 0;
	} else {
		IndicesManager *indiceAccidente = new IndicesManager("Archivos/accidentes.txt");
		claveAccidenteStr = indiceAccidente->obtenerValor(this->Valor);
		claveAccidenteStrAComp = indiceAccidente->obtenerValor(c->Valor);


		//Chequeo si la lineaClaveAComp existia en el archivo
		if (claveAccidenteStrAComp == "0") {
			//No existia la clave, devuelvo -1 para llevar la busqueda a un nodo incorrecto
			retVal = -1;
		} else if (claveAccidenteStr.compare(claveAccidenteStrAComp) < 0)
			retVal = -1;
		else if (claveAccidenteStr.compare(claveAccidenteStrAComp) == 0)
			retVal = 0;
		else
			retVal = 1;

	}

	return retVal;
}

