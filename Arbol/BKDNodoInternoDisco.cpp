/*
 * BKDNodoInternoDisco.cpp
 *
 *  Created on: May 31, 2012
 *      Author: vdiego
 */

#include "BKDNodoInternoDisco.h"

BKDNodoInternoDisco::BKDNodoInternoDisco(BKDManager* manager,int nroNodo, int capacidad, int nivel)
				: BKDNodoInterno(manager, nroNodo, capacidad, nivel)
{
}

BKDNodoInternoDisco::~BKDNodoInternoDisco()
{
}


/*********** Implementacion <Serializable> *************/
int BKDNodoInternoDisco::getLongitud()
{
	return 0;
}

int BKDNodoInternoDisco::serializar (Buffer* buffer, int posicion)
{
	return 0;
}

int BKDNodoInternoDisco::hidratar (Buffer* buffer, int posicion)
{
	return 0;
}

