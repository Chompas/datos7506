/*
 * Registro.h
 *
 *  Created on: 03/10/2010
 *      Author: Nacho
 */

#ifndef REGISTRO__H__
#define REGISTRO__H__
#include "Serializable.h"
#include "Buffer.h"

class Registro : public Serializable{
protected:
	char* dato;
	int longitudDato;

public:
	Registro();

	Registro(char* dato, int longitudDato);

	virtual ~Registro();

	char* getDato(int &longitudDato);

	void setDato(char* dato, int longitudDato);

	virtual int getLongitud();

	virtual int serializar (Buffer* buffer, int posicion);

	virtual int hidratar (Buffer* buffer, int posicion);

/***********************************************************
 * Metodos protegidos de la clase
 **********************************************************/

protected:
	char* clonarDato (char* dato, int longitudDato);
};

#endif /* REGISTRO__H__ */
