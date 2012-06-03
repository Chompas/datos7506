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
#include "IndiceDatos.h"


#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <time.h>

using namespace std;

namespace Pruebas
{
	int PruebaArbolB2();
}


IndiceDatos* indice;

void altaPorTeclado() {

	cout << "Ingresar Incidente con formato: linea;hora;falla;accidente;formacion" << endl;
	cout << "Ejemplo: Mitre;2011-10-2T12:12:45;no_cierra_puertas;incendio;15" << endl;
	cout << "Para finalizar la insercion ingresar * y presionar enter" << endl;
	string incidente;
	cin >> incidente;
	while (incidente != "*") {

		Incidente *unIncidente = new Incidente(incidente);

		//Aca en vez de imprimir el incidente hay que llamar a insertar arbol
		//unIncidente->imprimirIncidenteConClaves();
		indice->InsertarIncidente(*unIncidente);

		cin >> incidente;
	}

	cout << "=== Fin de la insercion ===" << endl;

}

void busquedaPorTeclado() {

	cout << "Ingresar Incidente a buscar con formato: linea;hora;falla;accidente;formacion" << endl;
	cout << "Para omitir claves en la busqueda ingresar un guion en la clave correspondiente" << endl;
	cout << "Ejemplo: Mitre;-;no_cierra_puertas;-;-  buscara todos las fallas de no_cierra_puertas de la linea Mitre" << endl;

	string incidente;
	cin >> incidente;

	cout << "Desea buscar por rango de fecha? s/n" << endl;
	string fechaOpcion;
	cin >> fechaOpcion;

	while (fechaOpcion != "s" && fechaOpcion != "n") {

			cin >> fechaOpcion;
	}

	if (fechaOpcion == "n") {
		//No hay busqueda por rango de fecha, creo el incidente y lo busco
		Incidente *unIncidente = new Incidente(incidente);

		//ACA HAY QUE AGREGAR LA LLAMADA A LA BUSQUEDA DE INCIDENTE Y PASAR unIncidente

		Incidente result(indice->BuscarIncidente(*unIncidente));



	} else {

		//La busqueda es por rango de fechas
		cout << "Ingresar fecha inicial con formato: AAAA-MM-DTHH:MM:SS " << endl;
		string fechaInicial;
		cin >> fechaInicial;

		cout << "Ingresar fecha final con formato: AAAA-MM-DTHH:MM:SS " << endl;
		string fechaFinal;
		cin >> fechaFinal;

		//Creo el incidente
		Incidente *unIncidente = new Incidente(incidente);
		//Modifico la clave horario a -1 para saber que se busca por rango
		unIncidente->horario = -1;

		//Convierto los horarios a timestamp
		struct tm tm;
		time_t epoch;
		if ( strptime(fechaInicial.c_str(), "%Y-%m-%dT%H:%M:%S", &tm) != NULL ) {
			epoch = mktime(&tm);
		}

		int horarioInicial = epoch;

		if ( strptime(fechaFinal.c_str(), "%Y-%m-%dT%H:%M:%S", &tm) != NULL ) {
			epoch = mktime(&tm);
		}

		int horarioFinal = epoch;

		//ACA HAGO LA LLAMADA A LA BUSQUEDA PERO PASANDO unIncidente, horarioInicial, horarioFinal


	}

}


int main(int argc, char* argv[])
{
	try
	{
		//***********************
		return Pruebas::PruebaArbolB2();
		//***********************

		indice = IndiceDatos::Crear("treeTest.UBA", 512);

		cout << "===========================" << endl;
		cout << "1. Insertar nuevo incidente" << endl;
		cout << "2. Buscar incidente" << endl;
		cout << "3. Finalizar" << endl;

		string opcion;

		cin >> opcion;

		while (opcion != "3") {
			if (opcion == "1") {
				altaPorTeclado();
			} else if (opcion == "2") {
				busquedaPorTeclado();
			}

			cout << "===========================" << endl;
			cout << "1. Insertar nuevo incidente" << endl;
			cout << "2. Buscar incidente" << endl;
			cout << "3. Finalizar" << endl;

			cin >> opcion;
		}

		delete indice;
	}
	catch (const std::exception& ex)
	{
		cerr << "Error: " << ex.what() <<endl;
	}
	catch (const std::string& ex)
	{
		cerr << "Error: " << ex <<endl;
	}
	catch (const char* ex)
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

}
