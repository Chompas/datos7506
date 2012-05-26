/*
 * Comparable.h
 *
 *  Created on: 03/10/2010
 *      Author: Nacho
 */

#ifndef COMPARABLE__H__
#define COMPARABLE__H__

class Comparable {
public:
	Comparable();

	virtual ~Comparable();

	virtual int compararCon(Comparable* objeto)=0;
};

#endif /* COMPARABLE__H__ */
