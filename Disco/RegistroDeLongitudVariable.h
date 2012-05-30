/*
 * RegistroDeLongitudVariable.h
 *
 *  Created on: 03/10/2010
 *      Author: Nacho
 */

#ifndef REGISTRODELONGITUDVARIABLE__H__
#define REGISTRODELONGITUDVARIABLE__H__
#include "Registro.h"

class RegistroDeLongitudVariable : public Registro{
public:
	RegistroDeLongitudVariable();

	RegistroDeLongitudVariable(char* dato, int longitudDato);

	virtual ~RegistroDeLongitudVariable();

	int getLongitud();
};

#endif /* REGISTRODELONGITUDVARIABLE__H__ */
