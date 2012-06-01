/*
 * RegistroDeLongitudFija.h
 *
 *  Created on: 05/10/2010
 *      Author: nacho
 */

#ifndef REGISTRODELONGITUDFIJA__H__
#define REGISTRODELONGITUDFIJA__H__
#include "Registro.h"

class RegistroDeLongitudFija : public Registro{
private:
	int longitud;

public:
	RegistroDeLongitudFija(int longitud);

	RegistroDeLongitudFija(char* dato, int longitudDato, int longitud);

	virtual ~RegistroDeLongitudFija();

	void setDato(char* dato, int longitudDato);

	int getLongitud();

	int serializar();

	int hidratar();

/***********************************************************
 * Metodos privados de la clase
 **********************************************************/

private:
	void setLongitud(int longitud);

	void setTipoRegistro ();

	bool longitudDatoValido(int longitudDato);
};

#endif /* REGISTRODELONGITUDFIJA__H__ */
