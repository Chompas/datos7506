/*
 * RegistroDeLongitudFija.cpp
 *
 *  Created on: 05/10/2010
 *      Author: nacho
 */

#include "RegistroDeLongitudFija.h"
#include "Constantes.h"
#include <cstring>

	RegistroDeLongitudFija::RegistroDeLongitudFija(int longitud){
		this->setLongitud(longitud);
		this->setTipoRegistro();
	}

	RegistroDeLongitudFija::RegistroDeLongitudFija(char* dato, int longitudDato, int longitud){
		this->setLongitud(longitud);
		this->setTipoRegistro();
		if (this->longitudDatoValido(longitudDato)) Registro(dato, longitudDato);
	}

	RegistroDeLongitudFija::~RegistroDeLongitudFija(){
		this->longitud = 0;
	}

	void RegistroDeLongitudFija::setDato(char* dato, int longitudDato){
		if ((dato != NULL) && (this->longitudDatoValido(longitudDato))){
			if (this->dato != NULL) delete [](this->dato);
			this->dato = this->clonarDato(dato, longitudDato);
			this->longitudDato = longitudDato;
		}
	}

	int RegistroDeLongitudFija::getLongitud(){
		return (this->longitud);
	}

/***********************************************************
 * Metodos privados de la clase
 **********************************************************/

	void RegistroDeLongitudFija::setLongitud(int longitud){
		this->longitud = 0; //valor invalido
		if (longitud >= LONGITUD_MINIMA_REGISTRO_DE_LONGITUD_FIJA)
			this->longitud = longitud;
	}

	void RegistroDeLongitudFija::setTipoRegistro(){
		this->tipoRegistro = RLFija;
	}

	bool RegistroDeLongitudFija::longitudDatoValido(int longitudDato){
		return ((longitudDato > 0) && (longitudDato < (this->longitud - LCCRLF4)));
	}
