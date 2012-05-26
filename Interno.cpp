#include "Interno.h"

Interno::Interno(){
	this -> nivel = 0;		  
	this -> capacidad = 0;
	this -> dimension = 0;
	this -> cantidad = 0;
	this -> claves = NULL;
	this -> hijos = NULL;
}

int Interno::agregarClave(std::string registro, Nodo * hd, Nodo * hi){
	return 0;
}

int Interno::agregarElemento(std::string registro){
	return 0;
}
	
int Interno::obtenerNivel(){
	return this -> nivel;
}

bool Interno::estaVacio(){
	return (this -> cantidad > 0);
}

std::string Interno::obtenerElemento(std::string registro){
	return "clave en Interno....";
}

Interno::~Interno (){
	//ver
}
