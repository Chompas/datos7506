/*
 * Archivo.h
 *
 *  Created on: 31/05/2012
 *      Author: nacho
 */

#ifndef ARCHIVO_H_
#define ARCHIVO_H_

//#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Bloque.h"
#include "Constantes.h"

class Archivo {
private:
	fstream archivo;
	fstream archivoControl;
	string nombreArchivo;
	int longitudBloque;
	int cantidadBloquesTotal;
	vector<int> bloquesLibres;

	void obtenerDatosDeControl();

	void guardarDatosDeControl();

public:
	Archivo(char* nombreArchivo, int longitudBloque = LBD);

	virtual ~Archivo();

	int obtenerNuevoBloque (Bloque* &bloque, int &idBloque);

	int obtenerBloque (Bloque* &bloque, int idBloque);

	int actualizarBloque (Bloque* bloque, int idBloque);

	int liberarBloque (int idBloque);

	int cantidadBloquesLibres ();

	int cantidadBloquesTotales ();

	int cantidadBloquesOcupados ();

	int obtenerBloqueLibre (Bloque* &bloque, int &idBloque);

};

#endif /* ARCHIVO_H_ */
