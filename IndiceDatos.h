/*
 * IndiceDatos.h
 *
 *  Created on: Jun 1, 2012
 *      Author: vdiego
 */

#ifndef INDICEDATOS_H_
#define INDICEDATOS_H_

#include "Arbol/BKDArbol.h"
#include "Claves/Incidente.h"
#include <string>

class IndiceDatos
{
protected:
	BKDArbol* m_arbolBKD;
public:
	virtual ~IndiceDatos();

	static IndiceDatos* Crear(const std::string filePath, int tamanioBloque);
	static IndiceDatos* Abrir(const std::string filePath, int tamanioBloque);

	Incidente BuscarIncidente(Incidente incidente);
	std::list<Incidente> BuscarPorRango(int formacionMin, int formacionMax);

	bool InsertarIncidente(Incidente incidente);
	bool ModificarIncidente(Incidente incidente);
	bool EliminarIncidente(int formacion);

};

#endif /* INDICEDATOS_H_ */
