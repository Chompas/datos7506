/*
 * BKDRegistro.h
 *
 *  Created on: May 31, 2012
 *      Author: vdiego
 */

#ifndef BKDREGISTRO_H_
#define BKDREGISTRO_H_

#include "../Disco/Serializable.h"
#include "BKDClaves.h"
#include <string>



class BKDRegistro : public Serializable
{
public:

	//!! Importante: La memoria debe ser liberada por el usuario !!
	virtual BKDClaveMultiple* GetClaveMultiple() const = 0;
	virtual BKDClave* GetClave() const = 0;
	virtual std::string ToString() const = 0;
	virtual BKDRegistro* Clonar() const = 0;
};


#endif /* BKDREGISTRO_H_ */
