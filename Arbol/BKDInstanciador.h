/*
 * BKDInstanciador.h
 *
 *  Created on: Jun 2, 2012
 *      Author: vdiego
 */

#ifndef BKDINSTANCIADOR_H_
#define BKDINSTANCIADOR_H_

#include "BKDRegistro.h"

class BKDInstanciador
{
public:
	virtual BKDRegistro* InstanciarRegistro() const = 0;
	virtual ~BKDInstanciador() {};
};


#endif /* BKDINSTANCIADOR_H_ */
