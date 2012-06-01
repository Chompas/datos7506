/*
 * RegistroDeLongitudVariable.cpp
 *
 *  Created on: 03/10/2010
 *      Author: Nacho
 */

#include <cstring>
#include "RegistroDeLongitudVariable.h"
#include "Constantes.h"

	RegistroDeLongitudVariable::RegistroDeLongitudVariable(){
		Registro();
		this->setTipoRegistro();
	}

	RegistroDeLongitudVariable::RegistroDeLongitudVariable(char* dato, int longitudDato){
		Registro(dato, longitudDato);
		this->setTipoRegistro();
	}

	RegistroDeLongitudVariable::~RegistroDeLongitudVariable(){

	}

	int RegistroDeLongitudVariable::getLongitud(){
		//la longitud del registro es la longitud del dato mas la longitud
		//del compo de control que es un entero que almacena la longitud del
		//dato, propiamentedicho.
		return (sizeof(int) + this->longitudDato);
	}

	void RegistroDeLongitudVariable::setTipoRegistro(){
		this->tipoRegistro = RLVariable;
	}
