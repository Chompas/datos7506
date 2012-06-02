/*
 * Archivo.h
 *
 *  Created on: 31/05/2012
 *      Author: nacho
 */

#ifndef ARCHIVO_H_
#define ARCHIVO_H_

#include <iostream>
#include <string>
#include <vector>

#include "Bloque.h"
#include "Constantes.h"

class Archivo {
private:
	fstream archivo;
	fstream archivoControl;
	char* nombreArchivo;
	int longitudBloque;
	int cantidadBloquesTotal;
	vector<int> bloquesLibres;

public:
	Archivo(char* nombreArchivo, int longitudBloque = LBD);

	virtual ~Archivo();

	Bloque* crearNuevoBloque ();

	int obtenerBloque (Bloque* bloque, int idBloque);

	int actualizarBloque (Bloque* bloque, int idBloque);

	int liberarBloque (int idBloque);

	int cantidadBloquesLibres ();

	int cantidadBloquesTotales ();

	int cabtidadBloquesOcupados ();

	int obtenerBloqueLibre ();

};

#endif /* ARCHIVO_H_ */
