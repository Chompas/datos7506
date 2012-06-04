/*
 * ClaveIndicente.cpp
 *
 *  Created on: Jun 1, 2012
 *      Author: chompas
 */

#include "ClaveIncidente.h"
#include "ClaveInt.h"
#include "ClaveAccidente.h"
#include "ClaveFalla.h"
#include "ClaveLinea.h"
#include <vector>
#include <stdio.h>
#include <string.h>
#include <sstream>

using namespace std;


ClaveIncidente::ClaveIncidente()
{
	this->m_dimension = -1;
	this->m_subclaves.clear();
}

ClaveIncidente::ClaveIncidente(const ClaveIncidente& clave)
{
	this->m_dimension = clave.m_dimension;
	this->m_subclaves.clear();

	for (std::vector<BKDClave*>::const_iterator i = clave.m_subclaves.begin(); i != clave.m_subclaves.end(); i++)
	{
		this->m_subclaves.push_back((*i)->Clonar());
	}
}

ClaveIncidente::ClaveIncidente(const Incidente& incidente)
{

	//Inserto en orden las claves en el vector
	ClaveLinea *claveLinea =  new ClaveLinea(incidente.linea);
	this->m_subclaves.push_back(claveLinea);
	ClaveInt *claveHorario = new ClaveInt(incidente.horario);
	this->m_subclaves.push_back(claveHorario);
	ClaveFalla *claveFalla = new ClaveFalla(incidente.falla);
	this->m_subclaves.push_back(claveFalla);
	ClaveAccidente *claveAccidente = new ClaveAccidente(incidente.accidente);
	this->m_subclaves.push_back(claveAccidente);
	ClaveInt *claveFormacion = new ClaveInt(incidente.formacion);
	this->m_subclaves.push_back(claveFormacion);


}


ClaveIncidente::~ClaveIncidente() {
	for (vector<BKDClave*>::const_iterator it = this->m_subclaves.begin(); it != this->m_subclaves.end(); ++it)
	{
		delete *it;

	}

	this->m_subclaves.clear();
}

int ClaveIncidente::GetDimension() const {

	return (int) m_subclaves.size();
}

// -1: this < clave
//  0: this == clave
//  1: this > clave
int ClaveIncidente::Comparar(const BKDClaveMultiple& clave) const
{

	vector<BKDClave*>::const_iterator it2 = clave.m_subclaves.begin();

	bool areEqual = true;
	int compare;
	int resultado = 0;

	for (vector<BKDClave*>::const_iterator it = this->m_subclaves.begin(); it != this->m_subclaves.end(); ++it)
	{
		compare = (*it)->Comparar(**it2);
		if (compare != 0 && areEqual == true) {
			areEqual = false;
			resultado = compare;
		}

		it2++;

	}

	return resultado;
}

BKDClave* ClaveIncidente::GetSubclave(int dimension) const {

	return m_subclaves[dimension];
}

string ClaveIncidente::ToString() const
{
	vector<BKDClave*>::const_iterator it;
	stringstream ss;
	ss << " [";

	for ( it=m_subclaves.begin() ; it < m_subclaves.end(); it++ ) {
		ss << "('";
		ss << (*it)->ToString();
		ss << "') , ";
	}

	ss << "] ";

	return ss.str();
}

int ClaveIncidente::serializar (Buffer* buffer, int posicion){
	char* stream = new char[20];
	char* ptr = stream;
	Buffer buffClave = Buffer();
	for (vector<BKDClave*>::const_iterator it = this->m_subclaves.begin(); it != this->m_subclaves.end(); ++it){
		(*it)->serializar(&buffClave, 0);
		int longitudStreamClave;
		memcpy(ptr, buffClave.getStream(longitudStreamClave), sizeof(int));
		ptr++;
		it++;
	}

	buffer->setStream(stream, 20);

	delete stream;

	return 0;
}

int ClaveIncidente::hidratar (Buffer* buffer, int posicion){
	//TO DO
	return 0;
}

int ClaveIncidente::getLongitud() {
	//TO DO
	return 0;
}
