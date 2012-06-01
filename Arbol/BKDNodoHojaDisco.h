/*
 * BKDNodoHojaDisco.h
 *
 *  Created on: May 31, 2012
 *      Author: vdiego
 */

#ifndef BKDNODOHOJADISCO_H_
#define BKDNODOHOJADISCO_H_

#include "BKDNodoHoja.h"
#include "../Disco/Serializable.h"

class BKDNodoHojaDisco : public BKDNodoHoja, public Serializable
{
public:
	BKDNodoHojaDisco(BKDManager* manager,int nroNodo, int capacidad, int nivel);
	virtual ~BKDNodoHojaDisco();

	//Implementacion de <Serializable>
	virtual int getLongitud();
	virtual int serializar (Buffer* buffer, int posicion);
	virtual int hidratar (Buffer* buffer, int posicion);

};

#endif /* BKDNODOHOJADISCO_H_ */
