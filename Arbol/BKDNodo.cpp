/*
 * BKDNodo.cpp
 *
 *  Created on: May 27, 2012
 *      Author: vdiego
 */

#include "BKDNodo.h"

int BKDNodo::GetNumeroNodo()
{
	return this->m_nro_nodo;
}

int BKDNodo::GetNivel()
{
	return this->m_nivel;
}

int BKDNodo::GetCapacidad()
{
	return this->m_capacidad;
}

bool BKDNodo::EsNodoHoja()
{
	return (this->m_nivel == 0);
}

