/*
 * ClaveIndicente.cpp
 *
 *  Created on: Jun 1, 2012
 *      Author: chompas
 */

#include "ClaveIncidente.h"
#include "ClaveInt.h"
#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;


ClaveIncidente::ClaveIncidente(const ClaveIncidente& clave)
{

}

ClaveIncidente::ClaveIncidente(Incidente& incidente)
{

	//Inserto en orden las claves en el vector
	ClaveInt *claveLinea =  new ClaveInt(incidente.linea);
	this->m_subclaves.push_back(claveLinea);
	ClaveInt *claveHorario = new ClaveInt(incidente.horario);
	this->m_subclaves.push_back(claveHorario);
	ClaveInt *claveFalla = new ClaveInt(incidente.falla);
	this->m_subclaves.push_back(claveFalla);
	ClaveInt *claveAccidente = new ClaveInt(incidente.accidente);
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

int ClaveIncidente::GetDimension() {

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

int ClaveIncidente::serializar (Buffer* buffer, int posicion){
	char* stream = new char[20];
	char* ptr = stream;
	Buffer* buffClave;
	for (vector<BKDClave*>::const_iterator it = this->m_subclaves.begin(); it != this->m_subclaves.end(); ++it){
		(*it)->serializar(buffClave, 0);
		int longitudStreamClave;
		memcpy(ptr, buffClave->getStream(longitudStreamClave), sizeof(int));
		ptr++;
		it++;
	}

	buffer->setStream(stream, 20);

	delete stream;

	return 0;
}

int ClaveIncidente::hidratar (Buffer* buffer, int posicion){
	return 0;
}
