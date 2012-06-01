/*
 * BKDNodoInternoDisco.h
 *
 *  Created on: May 31, 2012
 *      Author: vdiego
 */

#ifndef BKDNODOINTERNODISCO_H_
#define BKDNODOINTERNODISCO_H_

#include "BKDNodoInterno.h"
#include "../Disco/Serializable.h"

class BKDNodoInternoDisco : public BKDNodoInterno, public Serializable
{
public:
	BKDNodoInternoDisco(BKDManager* manager,int nroNodo, int capacidad, int nivel);
	virtual ~BKDNodoInternoDisco();

	//Implementacion de <Serializable>
	virtual int getLongitud();
	virtual int serializar (Buffer* buffer, int posicion);
	virtual int hidratar (Buffer* buffer, int posicion);
};

#endif /* BKDNODOINTERNODISCO_H_ */
