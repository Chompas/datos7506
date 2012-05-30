/*
 * Serializable.h
 *
 *  Created on: 03/10/2010
 *      Author: Nacho
 */

#ifndef SERIALIZABLE__H__
#define SERIALIZABLE__H__
#include "Buffer.h"

class Serializable {
public:
	Serializable(){};

	virtual ~Serializable(){};

	virtual int getLongitud()=0;

	virtual int serializar (Buffer* buffer, int posicion)=0;

	virtual int hidratar (Buffer* buffer, int posicion)=0;
};

#endif /* SERIALIZABLE__H__ */
