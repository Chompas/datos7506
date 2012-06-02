/*
 * ClaveIncidente.h
 *
 *  Created on: Jun 1, 2012
 *      Author: chompas
 */

#include "../Arbol/BKDClaves.h"
#include "Incidente.h"

#ifndef CLAVEINDICENTE_H_
#define CLAVEINCIDENTE_H_

class ClaveIncidente : public BKDClaveMultiple {
public:
	ClaveIncidente(const ClaveIncidente& clave);
	ClaveIncidente(Incidente incidente);
	virtual ~ClaveIncidente();

	virtual int Comparar(const BKDClaveMultiple& clave) const;
	virtual int GetDimension();
};

#endif /* CLAVEINDICENTE_H_ */
