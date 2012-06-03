/*
 * InstanciadorIncidentes.cpp
 *
 *  Created on: Jun 2, 2012
 *      Author: vdiego
 */

#include "InstanciadorIncidentes.h"

#include "Incidente.h"



InstanciadorIncidentes::InstanciadorIncidentes()
{ }

InstanciadorIncidentes::~InstanciadorIncidentes()
{ }


BKDRegistro* InstanciadorIncidentes::InstanciarRegistro() const
{
	return new Incidente();
}

