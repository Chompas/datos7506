/*
 * BKDNodoHojaDisco.h
 *
 *  Created on: May 31, 2012
 *      Author: vdiego
 */

#ifndef BKDNODOHOJADISCO_H_
#define BKDNODOHOJADISCO_H_

#include "BKDNodoHoja.h"
#include "../Disco/Bloque.h"

class BKDNodoHojaDisco : public BKDNodoHoja
{
public:
	BKDNodoHojaDisco(BKDManager* manager,int nroNodo, int capacidad, int nivel);

	virtual bool HayOverflow();
	virtual bool HayUnderflow();
	virtual int CantidadAMover(); //Indica la cantidad de registros que se deberían mover del nodo en overflow al nuevo hermano derecho

	virtual bool EscribirEnBloque(Bloque* bloque);
	virtual bool LeerDeBloque(Bloque* bloque);

};

#endif /* BKDNODOHOJADISCO_H_ */
