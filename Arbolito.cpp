/*
 * Arbolito.cpp
 *
 *  Created on: May 25, 2012
 *      Author: vdiego
 */

#include "Arbolito.h"
#include "Arbol/BKDArbol.h"
#include "Arbol/BKDManagerMemoria.h"
#include "Disco/Buffer.h"
#include "Disco/Bloque.h"
#include "Disco/Registro.h"
#include "Disco/RegistroDeLongitudVariable.h"
#include "Disco/RegistroDeLongitudFija.h"
#include "Disco/IndicesManager.h"
#include "Claves/Incidente.h"


#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <ctime>

using namespace std;

void altaPorTeclado() {

	cout << "Ingresar Incidente con formato: linea;hora;falla;accidente;formacion" << endl;
	cout << "Ejemplo: Mitre;2011-10-2T12:12:45;no_cierra_puertas;incendio;15" << endl;
	cout << "Para finalizar la insercion ingresar * y presionar enter" << endl;
	string incidente;
	cin >> incidente;
	while (incidente != "*") {

		Incidente *unIncidente = new Incidente(incidente);
		//Aca en vez de imprimir el incidente hay que llamar a insertar arbol
		unIncidente->imprimirIncidenteConClaves();

		cin >> incidente;
	}

	cout << "=== Fin de la insercion ===" << endl;

}

void prueba_lecturaDeArchivoIndice(){
	IndicesManager *indiceLineas = new IndicesManager("Archivos/lineas.txt");
	IndicesManager *indiceFallas = new IndicesManager("Archivos/fallas.txt");
	indiceLineas->imprimirCatalogo();
	indiceFallas->imprimirCatalogo();

}

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

int PruebaArbolB1()
{
	string filePath = "";
	int cap_hoja = 4;
	int cap_int = 3;

	//BKDArbol arbol = BKDArbol(filePath, cap_hoja, cap_int);
	BKDArbol* arbol = BKDArbol::CrearEnMemoria(cap_hoja, cap_int);

	cout << endl;
	cout << "======================== Comenzando pruebas de Arbol BKD ... =============";
	cout << endl << endl;

	cout << endl;
	cout << "Capacidad Hoja: " << cap_hoja << endl;
	cout << "Capacidad Interno: " << cap_int << endl;
	cout << "Archivo: \"" << filePath << "\"" << endl;
	cout << endl;

	cout << "Insertando valores 0..999";
	cout << endl;

	int cantRegs = 1000;
	vector<string> clavesInsert = vector<string>();

	for (int i = 0; i < cantRegs; i++)
	{
		std::stringstream ss;
		ss << i;
		clavesInsert.push_back(ss.str());
	}

	for (int i = 0; i < cantRegs; i++)
	//for (int i = cantRegs-1; i >= 0; i--)
	{
		TRegistro reg;
		reg.clave = clavesInsert[i];
		reg.valor = 0;

		if (!arbol->InsertarRegistro(reg))
		{
			cerr << "Error al insertar clave " << reg.clave << endl;
			delete arbol;
			return -1;
		}
	}

	cout << endl;
	cout << "Imprimiendo Arbol B+ ...";
	cout << endl << endl;

	arbol->DebugPrintAll();


	cout << endl;
	cout << "Buscando claves '-2', '0', '7', '99', '7', '666', '1249023'";
	cout << endl << endl;

	TRegistro regBusq = {"NULL",-1};
	bool res = arbol->BuscarRegistro("-2", regBusq);
	if (!res)
		cout << " NOPE ";
	else if (regBusq.clave != "-2")
		cout << " BAD ";
	else
		cout << " OK ";

	regBusq.clave = "NULL";
	regBusq.valor = 9999;
	res = arbol->BuscarRegistro("0", regBusq);
	if (!res)
		cout << " NOPE ";
	else if (regBusq.clave != "0")
		cout << " BAD ";
	else
		cout << " OK ";

	regBusq.clave = "NULL";
	regBusq.valor = 7;
	res = arbol->BuscarRegistro("7", regBusq);
	if (!res)
		cout << " NOPE ";
	else if (regBusq.clave != "7")
		cout << " BAD ";
	else
		cout << " OK ";

	regBusq.clave = "NULL";
	regBusq.valor = 99;
	res = arbol->BuscarRegistro("99", regBusq);
	if (!res)
		cout << " NOPE ";
	else if (regBusq.clave != "99")
		cout << " BAD ";
	else
		cout << " OK ";

	regBusq.clave = "NULL";
	regBusq.valor = 14;
	res = arbol->BuscarRegistro("7", regBusq);
	if (!res)
		cout << " NOPE ";
	else if (regBusq.clave != "7")
		cout << " BAD ";
	else
		cout << " OK ";

	regBusq.clave = "NULL";
	regBusq.valor = 654321;
	res = arbol->BuscarRegistro("666", regBusq);
	if (!res)
		cout << " NOPE ";
	else if (regBusq.clave != "666")
		cout << " BAD ";
	else
		cout << " OK ";

	regBusq.clave = "NULL";
	regBusq.valor = 777;
	res = arbol->BuscarRegistro("1249023", regBusq);
	if (!res)
		cout << " NOPE ";
	else if (regBusq.clave != "1249023")
		cout << " BAD ";
	else
		cout << " OK ";

	cout << endl;
	cout << endl;



	TClave claveInicio = "0";
	TClave claveFin = "500";

	cout << endl;
	cout << "Buscando rango de claves '"<< claveInicio << "' al '" << claveFin << "'";
	cout << endl << endl;

	list<TRegistro> resultadoRango = list<TRegistro>();
	arbol->BuscarPorRango(claveInicio, claveFin, resultadoRango);

	cout << "Cantidad de registros encontrados: " << resultadoRango.size() << endl;

	if (resultadoRango.size() > 0)
	{
		cout << "Registros: ";

		for (list<TRegistro>::iterator it = resultadoRango.begin(); it != resultadoRango.end(); it++)
		{
			cout << "{" << it->clave << "} ";
		}

		cout << endl;
	}

	cout << endl;



	cout << endl;
	cout << "======================== Fin pruebas de Arbol BKD ========================";
	cout << endl << endl;

	delete arbol;

	cout << ("Pulse enter para terminar.");
	cout << endl << endl;

	getchar();

	return 0;
}


int PruebaRegistros()
{
	string filePath = "RegTest.uba";

	cout << endl;
	cout << "======================== Comenzando pruebas de Registros ... =============";
	cout << endl << endl;

	cout << "Archivo: \"" << filePath << "\"" << endl << endl;

	int longRegistro = 25; //reg fijo de 25 Bytes
	cout << "Long de registro = fija de " << longRegistro << " Bytes" << endl << endl;

	cout << "Abriendo archivo..." << endl;
	fstream file;
	file.open(filePath.c_str(), fstream::binary | fstream::trunc | fstream::in | fstream::out);



	string dato1 = "string Nº 1";
	string dato2 = "int Nº 2";

	Registro r = Registro();
	cout << "Seteando dato [long:" << dato1.size() << "] = \"" << dato1 << "\"" << endl;
	r.setDato((char*)dato1.c_str(), dato1.size());

	cout << "Dato Seteado!!" << endl;

	Buffer buf = Buffer();

	cout << "Serealizando dato 1..." << endl;
	r.serializar(&buf, 2131245);

	cout << "Obteniendo stream..." << endl;
	int cantBytes = 0;
	char* bytes = buf.getStream(cantBytes);
	cout << "escribiendo a archivo..." << endl;
	file.write(bytes, cantBytes);
	file.flush();
	delete[] bytes;
	cout << "Escritura OK" << endl;



	cout << "Seteando dato [long:" << dato2.size() << "] = \"" << dato2 << "\"" << endl;
	r.setDato((char*)dato2.c_str(), dato2.size());

	buf = Buffer(NULL, longRegistro);
	r.serializar(&buf, 777);

	cantBytes = 0;
	bytes = buf.getStream(cantBytes);
	file.write(bytes, cantBytes);
	file.flush();
	delete[] bytes;


	//vuelvo al principio
	file.seekg (0, ios::beg);

	cout << "Leyendo dato 1..."<< endl;
	char* fbuf = new char[longRegistro];
	file.read(fbuf, longRegistro);
	buf = Buffer(fbuf, longRegistro);
	delete[] fbuf;
	r.hidratar(&buf, 5235);


	int tamDato = 0;
	char* d1 = r.getDato(tamDato);
	cout << "Dato 1 [long = " << tamDato << "] = \"" << d1 << "\"" << endl;
	delete[] d1;

	cout << "Leyendo dato 2..."<< endl;
	fbuf = new char[longRegistro];
	file.read(fbuf, longRegistro);
	buf = Buffer(fbuf, longRegistro);
	delete[] fbuf;
	r.hidratar(&buf, 5235);

	tamDato = 0;
	char* d2 = r.getDato(tamDato);
	cout << "Dato 2 [long = " << tamDato << "] = \"" << d2 << "\"" << endl;
	delete[] d2;


	file.close();

	cout << endl;
	cout << "======================== Fin pruebas de Registros ========================";
	cout << endl << endl;

	cout << ("Pulse enter para terminar.");
	cout << endl << endl;

	getchar();

	return 0;
}

int PruebaArbolDisco()
{
	string filePath = "TreeTest.uba";

	cout << endl;
	cout << "======================== Comenzando pruebas de Arbol en Disco ... =============";
	cout << endl << endl;

	cout << "Archivo: \"" << filePath << "\"" << endl << endl;

	cout << endl;
	cout << "======================== Fin pruebas de Arbol en Disco ========================";
	cout << endl << endl;

	cout << ("Pulse enter para terminar.");
	cout << endl << endl;

	getchar();

	return 0;
}


int main(int argc, char* argv[])
{
	/*try
	{
		return PruebaArbolB1();
		//return PruebaRegistros();
		//return PruebaArbolDisco();
	}
	catch (const std::exception& ex)
	{
		cerr << "Error: " << ex.what() <<endl;
	}
	catch (const std::string& ex)
	{
		cerr << "Error: " << ex <<endl;
	}
	catch (const int& ex)
	{
		cerr << "Error: código " << ex <<endl;

	}
	catch (...)
	{
		cerr << "Error no especificado" <<endl;
	}
*/
	cout << time(NULL) << endl;

	prueba_RegistroDeLongitudVariable();

	prueba_RegistroDeLongitudFija();

	prueba_lecturaDeArchivoIndice();

	altaPorTeclado();

}
