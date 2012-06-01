/*
 * IndiceDatos.cpp
 *
 *  Created on: Jun 1, 2012
 *      Author: vdiego
 */

#include "IndiceDatos.h"
#include <iostream>

using namespace std;

IndiceDatos::~IndiceDatos()
{
	delete this->m_arbolBKD;
}

IndiceDatos* IndiceDatos::Crear(const std::string filePath, int tamanioBloque)
{
	IndiceDatos* ret = new IndiceDatos();

	ret->m_arbolBKD = BKDArbol::CrearEnDisco(filePath, tamanioBloque);

	if (ret->m_arbolBKD == NULL)
		cerr << "Ocurrió un error al intentar crear el archivo de indice." << endl;

	return ret;
}

IndiceDatos* IndiceDatos::Abrir(std::string filePath, int tamanioBloque)
{
	IndiceDatos* ret = new IndiceDatos();

	ret->m_arbolBKD = BKDArbol::AbrirDeDisco(filePath);

	if (ret->m_arbolBKD == NULL)
		cerr << "Ocurrió un error al intentar acceder al archivo de indice." << endl;

	return ret;
}

Incidente IndiceDatos::BuscarIncidente(int formacion)
{
	BKDRegistro* inPtr;
	ClaveInt clave = ClaveInt(formacion);

	if (!this->m_arbolBKD->BuscarRegistro(clave, &inPtr))
		return Incidente();
	else
	{
		Incidente res = Incidente(*(Incidente*)inPtr);
		delete inPtr;
		return res;
	}
}

std::list<Incidente> IndiceDatos::BuscarPorRango(int formacionMin, int formacionMax)
{
	list<BKDRegistro*> outList;
	list<Incidente> result;
	ClaveInt claveMin = ClaveInt(formacionMin);
	ClaveInt claveMax = ClaveInt(formacionMax);

	if (!this->m_arbolBKD->BuscarPorRango(claveMin, claveMax, outList))
		return result;
	else
	{
		for(std::list<BKDRegistro*>::iterator it = outList.begin(); it != outList.end(); it++)
		{
			Incidente res = Incidente(*(Incidente*)(*it));
			result.push_back(res);
			delete *it;
			*it = NULL;
		}

		outList.clear();

		return result;
	}


	return std::list<Incidente>();
}

bool IndiceDatos::InsertarIncidente(Incidente incidente)
{
	return this->m_arbolBKD->InsertarRegistro(incidente);
}

bool IndiceDatos::ModificarIncidente(Incidente incidente)
{
	return this->m_arbolBKD->ModificarRegistro(incidente);
}

bool IndiceDatos::EliminarIncidente(int formacion)
{
	ClaveInt clave = ClaveInt(formacion);
	return this->m_arbolBKD->EliminarRegistro(clave);
}
