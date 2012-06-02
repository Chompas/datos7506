/*
 * BKDNodoInternoDisco.cpp
 *
 *  Created on: May 31, 2012
 *      Author: vdiego
 */

#include "BKDNodoInternoDisco.h"

BKDNodoInternoDisco::BKDNodoInternoDisco(BKDManager* manager,int nroNodo, int capacidad, int nivel)
				: BKDNodoInterno(manager, nroNodo, capacidad, nivel) { }

bool BKDNodoInternoDisco::EscribirEnBloque(Bloque* bloque)
{
	return false;
}

bool BKDNodoInternoDisco::LeerDeBloque(Bloque* bloque)
{
	return false;
}

