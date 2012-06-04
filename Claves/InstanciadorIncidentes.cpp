/*
 * InstanciadorIncidentes.cpp
 *
 *  Created on: Jun 2, 2012
 *      Author: vdiego
 */

#include "InstanciadorIncidentes.h"

#include "Incidente.h"
#include "ClaveIncidente.h"

//*******************
#include "ClaveInt.h"
//*******************


InstanciadorIncidentes::InstanciadorIncidentes()
{ }

InstanciadorIncidentes::~InstanciadorIncidentes()
{ }


BKDRegistro* InstanciadorIncidentes::InstanciarRegistro() const
{
	return new Incidente();
}

BKDClaveMultiple* InstanciadorIncidentes::InstanciarClave() const
{
	return new ClaveIncidente();
}

