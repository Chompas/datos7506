/*
 * Registro.h
 *
 *  Created on: 03/10/2010
 *      Author: Nacho
 */

#ifndef REGISTRO__H__
#define REGISTRO__H__

#include <string>
#include "Serializable.h"
#include "Buffer.h"

using namespace std;

class Registro : public Serializable{
protected:
	char* dato;
	int longitudDato;
	string tipoRegistro;

public:
	Registro();

	Registro(char* dato, int longitudDato);

	virtual ~Registro();

	char* getDato(int &longitudDato);

	string getTipoRegistro ();

	void setDato(char* dato, int longitudDato);

	virtual int getLongitud();

	virtual int serializar (Buffer* buffer, int posicion);

	virtual int hidratar (Buffer* buffer, int posicion);

/***********************************************************
 * Metodos protegidos de la clase
 **********************************************************/

protected:
	char* clonarDato (char* dato, int longitudDato);

	virtual void setTipoRegistro () = 0;

};

#endif /* REGISTRO__H__ */
