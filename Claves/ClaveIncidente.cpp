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
#include "../Comun/Utils.h"

#include <vector>
#include <stdio.h>
#include <string.h>
#include <sstream>

using namespace std;


ClaveIncidente::ClaveIncidente()
{
	this->m_dimension = -1;
	this->m_subclaves.clear();

	ClaveLinea *claveLinea =  new ClaveLinea(0);
	this->m_subclaves.push_back(claveLinea);
	ClaveInt *claveHorario = new ClaveInt(0);
	this->m_subclaves.push_back(claveHorario);
	ClaveFalla *claveFalla = new ClaveFalla(0);
	this->m_subclaves.push_back(claveFalla);
	ClaveAccidente *claveAccidente = new ClaveAccidente(0);
	this->m_subclaves.push_back(claveAccidente);
	ClaveInt *claveFormacion = new ClaveInt(0);
	this->m_subclaves.push_back(claveFormacion);
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

int ClaveIncidente::GetDimensiones() const
{
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

int ClaveIncidente::CompararPorSubclave(const BKDClave& subclave, const int dimension) const
{
	BKDClave* misubclave = this->GetSubclave(dimension);
	int res = misubclave->Comparar(subclave);
	delete misubclave;
	return res;
}

BKDClave* ClaveIncidente::GetSubclave(int dimension) const
{
	if (m_dimension >= (int)this->m_subclaves.size())
	{
		Utils::LogError(Utils::errSS << "Error al obtener subclave, dimension invalida: " << dimension);
		return NULL;
	}

	return m_subclaves[dimension];
}

string ClaveIncidente::ToString() const
{
	vector<BKDClave*>::const_iterator it;
	stringstream ss;
	ss << " [";

	int i = 0;
	for ( it=m_subclaves.begin() ; it < m_subclaves.end(); it++, i++ ) {
		ss << "('";
		if(i == 1)
		{
			ClaveInt* cli = (ClaveInt*)*it;
			ss << Utils::TimeStampToString(cli->Valor);
		}
		else
			ss << (*it)->ToString();
		ss << "') , ";
	}

	ss << "] ";

	return ss.str();
}



/*
int ClaveIncidente::serializar (Buffer* buffer, int posicion)
{
	char* stream = new char[5*sizeof(int)];
	char* ptr = stream;

	for (vector<BKDClave*>::const_iterator it = this->m_subclaves.begin(); it != this->m_subclaves.end(); it++)
	{
		ClaveInt* clave = (ClaveInt*)(*it);
		memcpy(ptr,&(clave->Valor), sizeof(int));
		ptr += sizeof(int);
	}

	buffer->setStream(stream, 5*sizeof(int));
	delete[] stream;

	return 0;
}
*/

int ClaveIncidente::serializar (Buffer* buffer, int posicion)
{
	char* stream = new char[this->getLongitud()];
	char* ptr = stream;

	for (vector<BKDClave*>::const_iterator it = this->m_subclaves.begin(); it != this->m_subclaves.end(); it++)
	{
		Buffer* buffClave = new Buffer();
		(*it)->serializar(buffClave, 0);
		int longitudStreamClave;
		char* scBuf = buffClave->getStream(longitudStreamClave);
		delete[] buffClave;

		memcpy(ptr,scBuf, longitudStreamClave);
		delete[] scBuf;

		ptr += longitudStreamClave;
	}

	buffer->setStream(stream, this->getLongitud());

	delete[] stream;

	return 0;
}

int ClaveIncidente::hidratar (Buffer* buffer, int posicion)
{
	if (buffer == NULL)
	{
		Utils::LogError(Utils::errSS << "Error al hidratar ClaveIncidente: Buffer NULL");
		return -1;
	}

	int longBuf = 0;
	char* buf = buffer->getStream(longBuf);

	if (buf == NULL)
	{
		Utils::LogError(Utils::errSS << "Error al hidratar ClaveIncidente: Stream NULL");
		return -1;
	}

	if (longBuf != this->getLongitud())
	{
		Utils::LogError(Utils::errSS << "Error al hidratar ClaveIncidente: longitud invalida");
		delete[] buf;
		return -1;
	}

	int aux = 0;
	char* ptr = buf;

	memcpy(&aux, ptr, sizeof(int));
	this->m_subclaves.push_back(new ClaveLinea(aux));
	ptr += sizeof(int);

	memcpy(&aux, ptr, sizeof(int));
	this->m_subclaves.push_back(new ClaveInt(aux));
	ptr += sizeof(int);

	memcpy(&aux, ptr, sizeof(int));
	this->m_subclaves.push_back(new ClaveFalla(aux));
	ptr += sizeof(int);

	memcpy(&aux, ptr, sizeof(int));
	this->m_subclaves.push_back(new ClaveAccidente(aux));
	ptr += sizeof(int);

	memcpy(&aux, ptr, sizeof(int));
	this->m_subclaves.push_back(new ClaveInt(aux));
	ptr += sizeof(int);

	delete[] buf;

	return 0;
}

int ClaveIncidente::getLongitud()
{
	return sizeof(int) * this->GetDimensiones();
}

BKDClaveMultiple* ClaveIncidente::Clonar() const
{
	return new ClaveIncidente(*this);
}


