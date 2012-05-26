#include <iostream>
#include <string>
#include "ArbolBMas.h"

ArbolBMas::ArbolBMas(int capaHoja, int capaInterno){
	this -> raiz 		= new Hoja(this -> capaHoja);
	this -> cantNiveles = 0;
	this -> capaHoja 	= capaHoja;
	this -> capaInterno = capaInterno;
}

void ArbolBMas::agregarRegistro(std::string registro){
	
	if (this -> cantNiveles <= 0) {
		//mi raiz es una Hoja
		
		this -> raiz -> agregarElemento(registro);
			
	} else {
		//mi raiz es un Interno
			
	}
	this -> raiz = new Interno(); 
}        

std::string ArbolBMas::obtenerRegistro(const std::string clave){
	return this -> raiz -> obtenerElemento(clave);	
}

bool ArbolBMas::estaVacio(){
	return (this -> cantNiveles = 0 && 
			(this -> raiz -> estaVacio() || this -> raiz == NULL));		
}

ArbolBMas::~ArbolBMas(){
	delete this -> raiz;
}
