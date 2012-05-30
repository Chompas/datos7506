/*
 * Definiciones.h
 *
 *  Created on: May 27, 2012
 *      Author: vdiego
 */

#ifndef DEFINICIONES_H_
#define DEFINICIONES_H_

#include <string>


typedef std::string TClave;
typedef long TValor;

//Representacion de un registro de datos
typedef struct
{
	TClave clave; //clave de busqueda
	TValor valor; //puntero a archivo de datos
} TRegistro;


#endif /* DEFINICIONES_H_ */
