/*
 * BKDNodoHojaDisco.cpp
 *
 *  Created on: May 31, 2012
 *      Author: vdiego
 */

#include "BKDNodoHojaDisco.h"

BKDNodoHojaDisco::BKDNodoHojaDisco(BKDManager* manager,int nroNodo, int capacidad, int nivel)
				: BKDNodoHoja(manager, nroNodo, capacidad, nivel)
{
}

BKDNodoHojaDisco::~BKDNodoHojaDisco()
{
}


/*********** Implementacion <Serializable> *************/
int BKDNodoHojaDisco::getLongitud()
{
	return 0;
}

int BKDNodoHojaDisco::serializar (Buffer* buffer, int posicion)
{
	return 0;
}

int BKDNodoHojaDisco::hidratar (Buffer* buffer, int posicion)
{
	return 0;
}

