/*
 * Incidente.h
 *
 *  Created on: May 30, 2012
 *      Author: chompas
 */

#include <string>
#include "../Disco/Serializable.h"
#include "../Arbol/BKDRegistro.h"
#include "ClaveInt.h"

using namespace std;

#ifndef INCIDENTE_H_
#define INCIDENTE_H_

class Incidente : public BKDRegistro
{
private:
	void insertarLinea(string lineaStr);
	void insertarHorario (string horarioStr);
	void insertarFalla (string fallaStr);
	void insertarAccidente (string accidenteStr);
	void insertarFormacion (string formacionStr);

public:
	//Variables
		int linea;
		int horario;
		int falla;
		int accidente;
		int formacion;

	Incidente();
	Incidente(string incAParsear);
	virtual ~Incidente();
	void imprimirIncidenteConClaves ();

	//Serializable
	virtual int getLongitud();
	virtual int serializar (Buffer* buffer, int posicion);
	virtual int hidratar (Buffer* buffer, int posicion);

	//!! Importante: La memoria debe ser liberada por el usuario !!
	virtual BKDClaveMultiple* GetClave() const;
	virtual std::string ToString() const;
	virtual std::string ToStringClaves() const;
	virtual BKDRegistro* Clonar() const;

};

#endif /* INCIDENTE_H_ */
