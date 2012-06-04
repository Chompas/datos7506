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

	ClaveIncidente();
	ClaveIncidente(const ClaveIncidente& clave);
	ClaveIncidente(const Incidente& incidente);

	virtual ~ClaveIncidente();

	virtual int Comparar(const BKDClaveMultiple& clave) const;
	virtual int CompararPorSubclave(const BKDClave& subclave, const int dimension) const;

	int GetDimensiones() const;

	int serializar (Buffer* buffer, int posicion);

	int hidratar (Buffer* buffer, int posicion);

	int getLongitud();

	BKDClave* GetSubclave(int dimension) const;

	string ToString() const;
	virtual BKDClaveMultiple* Clonar() const;
};

#endif /* CLAVEINDICENTE_H_ */
