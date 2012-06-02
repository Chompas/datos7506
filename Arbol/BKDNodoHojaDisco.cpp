/*
 * BKDNodoHojaDisco.cpp
 *
 *  Created on: May 31, 2012
 *      Author: vdiego
 */

#include "BKDNodoHojaDisco.h"

BKDNodoHojaDisco::BKDNodoHojaDisco(BKDManager* manager,int nroNodo, int capacidad, int nivel)
				: BKDNodoHoja(manager, nroNodo, capacidad, nivel) { }


bool BKDNodoHojaDisco::EscribirEnBloque(Bloque* bloque)
{
	return false;
}

bool BKDNodoHojaDisco::LeerDeBloque(Bloque* bloque)
{
	return false;
}
