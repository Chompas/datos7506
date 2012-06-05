/*
 * BKDNodoInternoDisco.h
 *
 *  Created on: May 31, 2012
 *      Author: vdiego
 */

#ifndef BKDNODOINTERNODISCO_H_
#define BKDNODOINTERNODISCO_H_

#include "BKDNodoInterno.h"
#include "../Disco/Bloque.h"

class BKDNodoInternoDisco : public BKDNodoInterno
{
public:
	BKDNodoInternoDisco(BKDManager* manager,int nroNodo, int capacidad, int nivel);

	virtual bool HayOverflow();
	virtual bool HayUnderflow();
	virtual int CantidadAMover(); //Indica la cantidad de registros que se deberían mover del nodo en overflow al nuevo hermano derecho

	virtual bool EscribirEnBloque(Bloque* bloque);
	virtual bool LeerDeBloque(Bloque* bloque);
};

#endif /* BKDNODOINTERNODISCO_H_ */
