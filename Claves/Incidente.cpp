/*
 * Incidente.cpp
 *
 *  Created on: May 30, 2012
 *      Author: chompas
 */

#include "Incidente.h"
#include "../Disco/IndicesManager.h"
#include "../Disco/Constantes.h"
#include "../Comun/Utils.h"

#include <vector>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "ClaveIncidente.h"

Incidente::Incidente()
{
	this->linea = -1;
	this->horario = -1;
	this->falla = -1;
	this->accidente = -1;
	this->formacion = -1;
}

Incidente::Incidente(string incAParsear) {

	vector<string> atributos;

	stringstream ss(incAParsear);
	string value;

	while(getline(ss,value,';')) {

	        atributos.push_back(value);
	}

	int i = 1;

	for (vector<string>::iterator it = atributos.begin(); it != atributos.end(); ++it)
	{
		//Hay que llamar dependiendo la clave a cada metodo para asignar los valores de los atributos
		switch (i)
		{
			//Linea Ferroviaria
		  	 case 1:
		  		insertarLinea(*it);
		  		break;

		  	 case 2:
		  		insertarHorario(*it);
		  		break;

		  	 case 3:
		  		insertarFalla(*it);
		  		break;

		  	 case 4:
		  		insertarAccidente(*it);
		  		break;

		  	 case 5:
		  		insertarFormacion(*it);
		  		break;

		  	 default:
		     	break;
		}

		i++;

	}

}

Incidente::~Incidente() {
	// TODO Auto-generated destructor stub
}

void Incidente::insertarLinea (string lineaStr) {

	//Si el usuario ingreso un guion asigno 0 para saber que esta clave no se usa
	//Eso se usa unicamente para las busquedas
	if (lineaStr == "-") {
		linea = 0;
	} else {
		IndicesManager *indiceLineas = new IndicesManager("Archivos/lineas.txt");
		int lineaClave = indiceLineas->obtenerClave(lineaStr);
		linea = lineaClave;
	}

}

void Incidente::insertarHorario (string horarioStr) {

	//Si el usuario ingreso un guion asigno 0 para saber que esta clave no se usa
	//Eso se usa unicamente para las busquedas
	if (horarioStr == "-") {
		horario = 0;
	} else {
		struct tm tm;
		time_t epoch;
		if ( strptime(horarioStr.c_str(), "%Y-%m-%dT%H:%M:%S", &tm) != NULL ) {
			epoch = mktime(&tm);
		}

		horario = epoch;
	}



}

void Incidente::insertarFalla (string fallaStr) {

	//Si el usuario ingreso un guion asigno 0 para saber que esta clave no se usa
	//Eso se usa unicamente para las busquedas
	if (fallaStr == "-") {
		falla = 0;
	} else {
		IndicesManager *indiceFallas = new IndicesManager("Archivos/fallas.txt");
		int fallaClave = indiceFallas->obtenerClave(fallaStr);
		falla = fallaClave;
	}

}

void Incidente::insertarAccidente (string accidenteStr) {

	//Si el usuario ingreso un guion asigno 0 para saber que esta clave no se usa
	//Eso se usa unicamente para las busquedas
	if (accidenteStr == "-") {
		accidente = 0;
	} else {
		IndicesManager *indiceAccidente = new IndicesManager("Archivos/accidentes.txt");
		int accidenteClave = indiceAccidente->obtenerClave(accidenteStr);
		accidente = accidenteClave;
	}

}

void Incidente::insertarFormacion (string formacionStr) {

	//Si el usuario ingreso un guion asigno 0 para saber que esta clave no se usa
	//Eso se usa unicamente para las busquedas
	if (formacionStr == "-") {
		formacion = 0;
	} else {
		formacion = atoi(formacionStr.c_str());
	}

}

void Incidente::imprimirIncidenteConClaves () {

	cout << "== Incidente == " << endl;
	cout << "Linea: " << linea << endl;
	cout << "Horario: " << horario << endl;
	cout << "Falla: " << falla << endl;
	cout << "Accidente: " << accidente << endl;
	cout << "Formacion: " << formacion << endl;

}

// Implementacion de Serializable

int Incidente::getLongitud(){

	//Longitud total de los 5 ints
	return 5 * sizeof(int);
}

int Incidente::serializar (Buffer* buffer, int posicion){
	char* stream = new char[getLongitud()];
	memset(stream, BASURA, getLongitud());
	char* ptr = stream;

	memcpy(ptr, &linea, sizeof(int));
	ptr += sizeof(int);
	memcpy(ptr, &horario, sizeof(int));
	ptr += sizeof(int);
	memcpy(ptr, &falla, sizeof(int));
	ptr += sizeof(int);
	memcpy(ptr, &accidente, sizeof(int));
	ptr += sizeof(int);
	memcpy(ptr, &formacion, sizeof(int));
	ptr += sizeof(int);

	buffer->setStream(stream, this->getLongitud());
	delete []stream;

	return 0;
}

int Incidente::hidratar (Buffer* buffer, int posicion){
	int longitudRegistro;
	char* stream = buffer->getStream(longitudRegistro);
	char* ptr = stream;

	memcpy(&linea, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(&horario, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(&falla, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(&accidente, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(&formacion, ptr, sizeof(int));
	ptr += sizeof(int);

	delete []stream;

	return 0;
}


// Implementacion de BKDRegistro

/*
BKDClave* Incidente::GetClave() const
{
	return new ClaveInt(this->formacion);
}
*/

BKDClaveMultiple* Incidente::GetClave() const
{
	return new ClaveIncidente(*this);
}

std::string Incidente::ToString() const
{
	string fallaStr;
	IndicesManager *indiceFalla = new IndicesManager("Archivos/fallas.txt");
	fallaStr = indiceFalla->obtenerValor(falla);
	delete indiceFalla;

	string horarioStr = Utils::TimeStampToString(this->horario);

	string accidenteStr;
	IndicesManager *indiceAccidente = new IndicesManager("Archivos/accidentes.txt");
	accidenteStr = indiceAccidente->obtenerValor(accidente);
	delete indiceAccidente;

	string lineaStr;
	IndicesManager *indiceLinea = new IndicesManager("Archivos/lineas.txt");
	lineaStr = indiceLinea->obtenerValor(linea);
	delete indiceLinea;


	stringstream ss;
	ss << " [";
	ss << "('";
	ss << lineaStr;
	ss << "') , ";
	ss << "('";
	ss << horarioStr;
	ss << "') , ";
	ss << "('";
	ss << fallaStr;
	ss << "') , ";
	ss << "('";
	ss << accidenteStr;
	ss << "') , ";
	ss << "('";
	ss << this->formacion;
	ss << "')";
	ss << "] ";

	return ss.str();
}

std::string Incidente::ToStringClaves() const
{

	stringstream ss;
	ss << " [";
	ss << "('";
	ss << this->linea;
	ss << "') , ";
	ss << "('";
	ss << this->horario;
	ss << "') , ";
	ss << "('";
	ss << this->falla;
	ss << "') , ";
	ss << "('";
	ss << this->accidente;
	ss << "') , ";
	ss << "('";
	ss << this->formacion;
	ss << "')";
	ss << "] ";

	return ss.str();
}

BKDRegistro* Incidente::Clonar() const
{
	return new Incidente(*this);
}

