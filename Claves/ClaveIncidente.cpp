/*
 * ClaveIndicente.cpp
 *
 *  Created on: Jun 1, 2012
 *      Author: chompas
 */

#include "ClaveIncidente.h"
#include "ClaveInt.h"
#include <vector>

using namespace std;

ClaveIncidente::ClaveIncidente(const ClaveIncidente& clave) {

}

ClaveIncidente::ClaveIncidente(Incidente incidente) {

	//Inserto en orden las claves en el vector
	ClaveInt claveLinea = ClaveInt(incidente.linea);
	this->m_subclaves.push_back(&claveLinea);
	ClaveInt claveHorario = ClaveInt(incidente.horario);
	this->m_subclaves.push_back(&claveHorario);
	ClaveInt claveFalla = ClaveInt(incidente.falla);
	this->m_subclaves.push_back(&claveFalla);
	ClaveInt claveAccidente = ClaveInt(incidente.accidente);
	this->m_subclaves.push_back(&claveAccidente);
	ClaveInt claveFormacion = ClaveInt(incidente.formacion);
	this->m_subclaves.push_back(&claveFormacion);


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

	for (vector<BKDClave*>::const_iterator it = this->m_subclaves.begin(); it != this->m_subclaves.end(); ++it)
	{
		compare = (*it)->Comparar(**it2);
		if (compare != 0) {
			areEqual = false;
		}

		it2++;

	}

	if (areEqual == true) {
		return 0;
	} else {
		return 1;
	}
}

