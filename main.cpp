#include <iostream>
#include <fstream>
//clases
#include "ArbolBMas.h"

using namespace std;

#define CAPAHOJA	3
#define CAPAINTERNO	2

void pruebaArbolBMas(){	
	ArbolBMas * miBMas = new ArbolBMas(CAPAHOJA, CAPAINTERNO);
	cout << "generacion del arbol B+ ok" << endl;
	
	miBMas -> agregarRegistro("1;00:30-01:40 12/02/2012;153;-1;1");
	miBMas -> agregarRegistro("2;00:30-01:40 12/02/2012;-1;120;2");
	miBMas -> agregarRegistro("3;00:30-01:40 12/02/2012;13;-1;2");
	miBMas -> agregarRegistro("2;00:30-01:40 12/02/2012;-1;-1;3");
	
	cout << "carga del arbol B+ ok" << endl;
	if (!miBMas -> estaVacio()) 
		cout << miBMas -> obtenerRegistro("1;00:30-01:40 12/02/2012;153;-1;1") << endl;
	
	cout << "consulta del arbol B+ ok" << endl;
		
}

int main() {

	pruebaArbolBMas();
	cout << "Principal fin ok" << endl;
	return 0;
	
}
