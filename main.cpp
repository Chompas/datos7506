#include <iostream>
#include <fstream>
//clases
#include "ArbolBMas.h"
#include "Buffer.h"
#include "Registro.h"
#include "RegistroDeLongitudVariable.h"
#include "RegistroDeLongitudFija.h"

using namespace std;

#define CAPAHOJA	3
#define CAPAINTERNO	2


using namespace std;

void prueba_RegistroDeLongitudVariable(){
	Registro* registro1 = new RegistroDeLongitudVariable();
	int longitudDato = 9; //entero + cadena de 5 bytes
	char* stream = new char[longitudDato];
	char* ptr = stream;
	int edad = 28;

	cout << "Inicio de prueba de RegistroDeLongitudVariable" << endl;
	memcpy(ptr, &edad, sizeof(int));
	ptr += sizeof(int);
	memcpy(ptr, "Nacho", 5);

	registro1->setDato(stream, longitudDato);

	int longitudDato2;
	char* dato = registro1->getDato(longitudDato2);
	ptr = dato;

	int edad2;
	char* nombre = new char[6];
	memcpy(&edad2, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(nombre, ptr, 5);
	nombre[5] = '\0';

	cout << "edad obtenida desde getDato(): " << edad2 << endl;
	cout << "nombre obtenido desde getDato(): " << string(nombre) << endl;

	Registro* registro2 = new RegistroDeLongitudVariable();
	Buffer* buffer = new Buffer();
	registro1->serializar(buffer, 0);
	registro2->hidratar(buffer, 0);

	cout << "longitud del registro obtenido despues de serializar e hidratar: " << registro2->getLongitud() << endl;

	delete []dato;
	delete []stream;
	delete []nombre;
	delete registro1;
	delete registro2;
	delete buffer;

	cout << "Fin de la prueba" << endl;
}

void prueba_RegistroDeLongitudFija(){
	Registro* registro1 = new RegistroDeLongitudFija(20);
	int longitudDato = 9; //entero + cadena de 5 bytes
	char* stream = new char[longitudDato];
	char* ptr = stream;
	int edad = 28;

	cout << "Inicio de prueba de RegistroDeLongitudFija" << endl;
	memcpy(ptr, &edad, sizeof(int));
	ptr += sizeof(int);
	memcpy(ptr, "Nacho", 5);

	registro1->setDato(stream, longitudDato);

	int longitudDato2;
	char* dato = registro1->getDato(longitudDato2);
	ptr = dato;

	int edad2;
	char* nombre = new char[6];
	memcpy(&edad2, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(nombre, ptr, 5);
	nombre[5] = '\0';

	cout << "edad obtenida desde getDato(): " << edad2 << endl;
	cout << "nombre obtenido desde getDato(): " << string(nombre) << endl;

	Registro* registro2 = new RegistroDeLongitudFija(20);
	Buffer* buffer = new Buffer();
	registro1->serializar(buffer, 0);
	registro2->hidratar(buffer, 0);

	cout << "longitud del registro obtenido despues de serializar e hidratar: " << registro2->getLongitud() << endl;

	int longitudDato3;
	char* dato2 = registro2->getDato(longitudDato3);
	ptr = dato2;

	memcpy(&edad2, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(nombre, ptr, 5);
	nombre[5] = '\0';

	cout << "edad obtenida desde getDato() despues de serializar e hidratar: " << edad2 << endl;
	cout << "nombre obtenido desde getDato() despues de serializar e hidratar: " << string(nombre) << endl;

	delete []dato;
	delete []dato2;
	delete []stream;
	delete []nombre;
	delete registro1;
	delete registro2;
	delete buffer;

	cout << "Fin de la prueba" << endl;
}

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

	delete miBMas;
		
}

int main (){

	prueba_RegistroDeLongitudVariable();

	prueba_RegistroDeLongitudFija();

	pruebaArbolBMas();

//	cout << "Principal fin ok" << endl;

	return 0;
}

