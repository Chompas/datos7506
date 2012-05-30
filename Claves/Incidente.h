/*
 * Incidente.h
 *
 *  Created on: May 30, 2012
 *      Author: chompas
 */

#include <string>
#include "../Disco/Serializable.h"

using namespace std;

#ifndef INCIDENTE_H_
#define INCIDENTE_H_

class Incidente : public Serializable{

private:
	void insertarLinea(string lineaStr);
	void insertarHorario (string horarioStr);
	void insertarFalla (string fallaStr);
	void insertarAccidente (string accidenteStr);
	void insertarFormacion (string formacionStr);

public:
	Incidente();
	Incidente(string incAParsear);
	virtual ~Incidente();
	void imprimirIncidenteConClaves ();

	//Serializable
	int getLongitud();
	int serializar (Buffer* buffer, int posicion);
	int hidratar (Buffer* buffer, int posicion);

	//Variables
	int linea;
	int horario;
	int falla;
	int accidente;
	int formacion;
};

#endif /* INCIDENTE_H_ */
