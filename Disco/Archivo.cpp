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

void Archivo::obtenerDatosDeControl(){
	char* buff = new char[4];
	int cantidadBloquesLibres;

	this->archivoControl.seekg (0, ios::beg);
	this->archivoControl.read (buff, 4);
	memcpy (buff, &this->longitudBloque, sizeof(int));
	this->archivoControl.read (buff, 4);
	memcpy (buff, &this->cantidadBloquesTotal, sizeof(int));
	this->archivoControl.read (buff, 4);
	memcpy (buff, &cantidadBloquesLibres, sizeof(int));

	for (int contador = 1; contador <= cantidadBloquesLibres; contador++){
		int idBloqueLibre;
		this->archivoControl.read (buff, 4);
		memcpy (buff, &idBloqueLibre, sizeof(int));
		this->bloquesLibres.push_back(idBloqueLibre);
	}

	delete []buff;
}

void Archivo::persistirDatosDeControl(){
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

	this->archivoControl.seekp (0, ios::beg);
	this->archivoControl.write (stream, longitudStream);

	delete []stream;
}

Archivo::Archivo(char* nombreArchivo, int longitudBloque) {
	this->archivo.open (nombreArchivo, fstream::binary | fstream::in | fstream::out);
	this->nombreArchivo = nombreArchivo;
	this->archivoControl.open ("control.dat", fstream::binary | fstream::in | fstream::out);
	this->longitudBloque = longitudBloque;
	this->cantidadBloquesTotal = 0;
	obtenerDatosDeControl();
}

Archivo::~Archivo() {
	this->archivo.close();
	this->archivoControl.close();
	persistirDatosDeControl();
}

Bloque* Archivo::obtenerNuevoBloque (int &idBloque){
	Bloque* bloque = new Bloque (this->longitudBloque);

	this->archivo.seekg (0, ios::end);
	if (this->archivo.tellg() >-1)
		idBloque = (this->archivo.tellg() / this->longitudBloque);
	else
		idBloque = 0;

	this->cantidadBloquesTotal++;

	return bloque;
}

int Archivo::obtenerBloque (Bloque* bloque, int idBloque){
	if (idBloque < this->cantidadBloquesTotal){
		char* stream = new char[this->longitudBloque];
		int posicion = idBloque * this->longitudBloque;

		this->archivo.seekg(posicion);
		this->archivo.read(stream, this->longitudBloque);

		Buffer* buffer = new Buffer (stream, this->longitudBloque);
		bloque = new Bloque (buffer, this->longitudBloque);

		delete (stream);
		delete (buffer);

		return 0;
	}else
		return 1; //no existe bloque
}

int Archivo::actualizarBloque (Bloque* bloque, int idBloque){
	int posicion = idBloque * this->longitudBloque;

	this->archivo.seekp (posicion);
	Buffer* buffer = NULL;
	bloque->serializar(buffer, this->longitudBloque);
	this->archivo.write(buffer->getStream(this->longitudBloque), this->longitudBloque);

	delete buffer;

	return 0;
}

int Archivo::liberarBloque (int idBloque){
	std::vector<int>::iterator it = this->bloquesLibres.begin();

	if (this->bloquesLibres.size() > 0){

		//verifica si ya est libre
		unsigned int contador = 0;
		while ((*it != idBloque) and (contador < this->bloquesLibres.size())){
			it++;
			contador++;
		}
	}

	if (*it == idBloque)
		return 1;
	else{
		this->bloquesLibres.push_back(idBloque);
		return 0;
		}
}

int Archivo::cantidadBloquesLibres (){
	return (this->bloquesLibres.size());
}

int Archivo::cantidadBloquesTotales (){
	return (this->cantidadBloquesTotal);
}

int Archivo::cabtidadBloquesOcupados (){
	return  (this->cantidadBloquesTotal - cantidadBloquesLibres());
}

int Archivo::obtenerBloqueLibre (Bloque *bloque, int &idBloque){
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
