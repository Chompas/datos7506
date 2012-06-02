/*
 * Archivo.cpp
 *
 *  Created on: 31/05/2012
 *      Author: nacho
 */

#include <fstream>
#include "Archivo.h"
#include "Buffer.h"

Archivo::Archivo(char* nombreArchivo, int longitudBloque) {
	this->archivo.open (nombreArchivo, fstream::binary | fstream::in | fstream::out);
	this->nombreArchivo = nombreArchivo;
	this->archivoControl.open ("control.dat", fstream::binary | fstream::in | fstream::out);
	this->longitudBloque = longitudBloque;
}

Archivo::~Archivo() {
	this->archivo.close();
	this->archivoControl.close();
}

Bloque* Archivo::crearNuevoBloque (int &idBloque){
	Bloque* bloque = new Bloque (this->longitudBloque);

	this->archivo.seekg (0, ios::end);
	idBloque = (this->archivo.tellg() / this->longitudBloque) - 1;

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
