#include "Hoja.h"
#include "Interno.h"

Hoja::Hoja(int capacidad){
	this -> nivel = 0;		  
	this -> capacidad = capacidad;
	this -> dimension = 0;
	this -> cantidad = 0;
	this -> registros = new std::string[capacidad];
	this -> vecino = NULL;
}

int Hoja::agregarElemento(std::string registro){
	if (this -> cantidad < this -> capacidad){
		//se agrega sin problemas
		return insertarElemento(registro);		
	} else {
		//devuelve -1 si esta llena
		return -1;
	}
}

int Hoja::insertarElemento(std::string registro){
	if (this -> registros[0].empty()) {
		this -> registros[0] = registro;
		
	} else {
		std::string * buffer = new std::string[this -> capacidad - 1];
		int i = 0,j = 0;
		while (i < this -> capacidad) {			
			if (this -> registros[i].compare(registro) > 0) 
				buffer[j++] = this -> registros[i];			
			i++;
		}
		
		int k = i - j;
		this -> registros[k++] = registro;
		
		for (int j = k, i = 0; j < this -> capacidad; j++, i++){
			this -> registros[j] = buffer[i];
		}			
	}
	
	this -> cantidad++;
	return 0;
}

void Hoja::distribuir(Nodo* unaHoja, std::string registro){
	int numElem = (this -> capacidad + 1) / 2;
	numElem += (this -> capacidad + 1) % 2;
	
	std::string*  buffer = new std::string[this -> capacidad + 1];
	for (int i = 0, j = 0; i < this -> capacidad; i++){
			if (this -> registros[i].compare(registro) < 0)
				buffer[j++] = registro;
			buffer[j++] = this -> registros[i];
	}
	
	for (int i = numElem + 1; i < this -> capacidad + 1; i++){
		unaHoja -> agregarElemento(buffer[i]);
		this -> registros[i].clear();
	}		
}
	
int Hoja::obtenerNivel(){
	return this -> nivel;
}

bool Hoja::estaVacio(){
	return (this -> cantidad > 0);
}

std::string Hoja::obtenerElemento(std::string registro){
	return "registro en Hoja....";
}

Hoja::~Hoja (){
	//ver
	delete [] this -> registros;
	delete [] this -> vecino;
}
