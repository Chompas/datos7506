/*
 * Archivo.cpp
 *
 *  Created on: 31/05/2012
 *      Author: nacho
 */

#include <fstream>
#include <stdio.h>
#include <string.h>
#include "Archivo.h"
#include "Buffer.h"

#include <iostream>

void Archivo::obtenerDatosDeControl(){
	this->archivoControl.open (NOMBRE_ARCHIVO_CONTROL.c_str(), fstream::binary | fstream::in);

	this->archivoControl.seekg (0, ios::end);
	if (this->archivoControl.tellg() > -1){
		char* buff = new char[4];
		int cantidadBloquesLibres = 0;

		this->archivoControl.seekg (0, ios::beg);
		this->archivoControl.read (buff, 4);
		memcpy (&this->longitudBloque, buff, sizeof(int));
		this->archivoControl.read (buff, 4);
		memcpy (&this->cantidadBloquesTotal, buff , sizeof(int));
		this->archivoControl.read (buff, 4);
		memcpy (&cantidadBloquesLibres, buff, sizeof(int));

		for (int contador = 1; contador <= cantidadBloquesLibres; contador++){
			int idBloqueLibre;
			this->archivoControl.read (buff, 4);
			memcpy (&idBloqueLibre, buff, sizeof(int));
			this->bloquesLibres.push_back(idBloqueLibre);
		}

		delete []buff;
	}

	this->archivoControl.close();
}

void Archivo::guardarDatosDeControl(){
	this->archivoControl.open (NOMBRE_ARCHIVO_CONTROL.c_str(), fstream::binary | fstream::out | fstream::trunc);

	int longitudStream = LCCAC + this->cantidadBloquesLibres() * 4;
	char* stream = new char[longitudStream];
	char* ptr = stream;

	memcpy(ptr, &this->longitudBloque, sizeof(int));
	ptr += sizeof(int);

	memcpy(ptr, &this->cantidadBloquesTotal, sizeof(int));
	ptr += sizeof(int);

	int cBL = cantidadBloquesLibres();
	memcpy(ptr, &cBL, sizeof(int));
	ptr += sizeof(int);

	std::vector<int>::iterator it = this->bloquesLibres.begin();
	for (int contador = 0; contador < cantidadBloquesLibres(); contador++){
		memcpy(ptr, &(*it), sizeof(int));
		ptr += sizeof(int);
		it++;
	}

	this->archivoControl.write (stream, longitudStream);

	this->archivoControl.close();

	delete []stream;
}

Archivo::Archivo(char* nombreArchivo, int longitudBloque) {
	this->nombreArchivo = nombreArchivo;
	this->longitudBloque = longitudBloque;
	this->cantidadBloquesTotal = 0;
	obtenerDatosDeControl();
}

Archivo::~Archivo() {
	guardarDatosDeControl();
}

int Archivo::obtenerNuevoBloque (Bloque* &bloque, int &idBloque){
	bloque = new Bloque (this->longitudBloque);
	if (this->cantidadBloquesTotal == 0)
		idBloque = 0;
	else
		idBloque = this->cantidadBloquesTotal;

	return 0;
}

int Archivo::obtenerBloque (Bloque* &bloque, int idBloque){
	if ((idBloque < this->cantidadBloquesTotal) and (idBloque > -1)){
		this->archivo.open (this->nombreArchivo.c_str(), fstream::binary | fstream::in);

		char* stream = new char[this->longitudBloque];
		int posicion = idBloque * this->longitudBloque;

		this->archivo.seekg(posicion);
		this->archivo.read(stream, this->longitudBloque);

		Buffer* buffer = new Buffer (stream, this->longitudBloque);
		bloque = new Bloque (buffer, this->longitudBloque);

		delete []stream;
		delete (buffer);

		this->archivo.close();

		return 0;
	}else
		return 1; //no existe bloque
}

int Archivo::actualizarBloque (Bloque* bloque, int idBloque){
	int posicion = idBloque * this->longitudBloque;

	if (idBloque <= this->cantidadBloquesTotal){
		this->archivo.open (this->nombreArchivo.c_str(), fstream::binary | fstream::out);

		this->archivo.seekp (posicion);
		Buffer* buffer = new Buffer();
		bloque->serializar(buffer, 0);
		int longitudBuffer = 0;
		char* stream = buffer->getStream(longitudBuffer);
		this->archivo.write(stream, this->longitudBloque);

		delete []stream;
		delete buffer;

		if (idBloque == this->cantidadBloquesTotal)
			this->cantidadBloquesTotal++;

		this->archivo.close();

		return 0;
	}else
		return 1;
}

int Archivo::liberarBloque (int idBloque){
	std::vector<int>::iterator it = this->bloquesLibres.begin();

	if (cantidadBloquesLibres() > 0){
		//verifica si ya esta libre
		int contador = 0;
		bool existe = false;
		while ((!existe) && (contador < cantidadBloquesLibres ())){
			if ((*it) == idBloque)
				existe = true;
			it++;
			contador++;
		}

		if (existe)
			return 1;
	}

	if (idBloque > cantidadBloquesTotales() - 1)
		return 2;

	this->bloquesLibres.push_back(idBloque);

	return 0;
}

int Archivo::cantidadBloquesLibres (){
	return (this->bloquesLibres.size());
}

int Archivo::cantidadBloquesTotales (){
	return (this->cantidadBloquesTotal);
}

int Archivo::cantidadBloquesOcupados (){
	return  (this->cantidadBloquesTotal - cantidadBloquesLibres());
}

int Archivo::obtenerBloqueLibre (Bloque* &bloque, int &idBloque){
	if (cantidadBloquesLibres() > 0){
		std::vector<int>::iterator it = this->bloquesLibres.begin();
		//entrega el primer bloque libre de la lista
		idBloque = *it;
		obtenerBloque (bloque, idBloque);
		//una vez entregado lo elimina de la lista
		this->bloquesLibres.erase(it);
		return 0;
	}else
		return 1;

}
