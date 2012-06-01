/*
 * Registro.cpp
 *
 *  Created on: 03/10/2010
 *      Author: Nacho
 */

#include "Registro.h"
#include "Constantes.h"
#include <cstring>

	Registro::Registro(){
		this->dato = NULL;
		this->longitudDato = 0;
	}

	Registro::Registro(char* dato, int longitudDato){
		this->setDato(dato, longitudDato);
	}

	Registro::~Registro(){
		if (this->dato != NULL) delete [](this->dato);
		this->dato = NULL;
		this->longitudDato = 0;
	}

	char* Registro::getDato(int &longitudDato){
		longitudDato = this->longitudDato;
		return (this->clonarDato(this->dato, this->longitudDato));
	}

	string Registro::getTipoRegistro (){
		return this->tipoRegistro;
	}

	void Registro::setDato(char* dato, int longitudDato){
		if ((dato != NULL) && (longitudDato > 0)){
			if (this->dato != NULL) delete [](this->dato);
			this->dato = this->clonarDato(dato, longitudDato);
			this->longitudDato = longitudDato;
		}
	}

	int Registro::getLongitud(){
		return 0;
	}

	int Registro::serializar (Buffer* buffer, int posicion){
		char* stream = new char[this->getLongitud()];
		memset(stream, BASURA, this->getLongitud());
		char* ptr = stream;

		memcpy(ptr, &(this->longitudDato), sizeof(int));
		ptr += sizeof(int);
		memcpy(ptr, this->dato, this->longitudDato);

		buffer->setStream(stream, this->getLongitud());
		delete []stream;

		return 0;
	}

	int Registro::hidratar (Buffer* buffer, int posicion){
		int longitudRegistro;
		int longitudDato;
		char* stream = buffer->getStream(longitudRegistro);
		char* ptr = stream;

		memcpy(&longitudDato, ptr, sizeof(int));
		ptr += sizeof(int);
		char* dato = new char[longitudDato];
		memcpy(dato, ptr, longitudDato);
		this->setDato(dato, longitudDato);

		delete []stream;
		delete []dato;

		return 0;
	}

/***********************************************************
 * Metodos protegidos de la clase
 **********************************************************/

	char* Registro::clonarDato (char* dato, int longitudDato){
		char* datoClonado = new char[longitudDato];
		memcpy (datoClonado, dato, longitudDato);
		return datoClonado;
	}

	void Registro::setTipoRegistro (){

	}
