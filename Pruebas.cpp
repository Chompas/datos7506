/*
 * Pruebas.cpp
 *
 *  Created on: Jun 1, 2012
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
#include "Arbol/BKDRegistro.h"

#include "IndiceDatos.h"

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <ctime>

using namespace std;


namespace Pruebas
{

	static int PruebaArbolB2()
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

		int cantRegs = 70;
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
			Incidente reg = Incidente();
			reg.formacion = i;
			reg.linea = i/2;

			if (!arbol->InsertarRegistro(reg))
			{
				cerr << "Error al insertar clave " << reg.formacion << endl;
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

		int busq[] = {-2, 0, 7, 99, 7, 666, 1249023};

		for(int i = 0; i < 7; i++)
		{
			ClaveInt claveBusqueda = ClaveInt(busq[i]);
			BKDRegistro* regBusq = NULL;

			bool res = arbol->BuscarRegistro(claveBusqueda, &regBusq);

			if (!res)
				cout << " NOPE ";
			else if (((Incidente*)regBusq)->formacion != busq[i])
				cout << " BAD ";
			else
				cout << " OK ";

			delete regBusq;
		}

		cout << endl;
		cout << endl;



		ClaveInt claveInicio = ClaveInt(0);
		ClaveInt claveFin = ClaveInt(500);

		cout << endl;
		cout << "Buscando rango de claves '"<< claveInicio.Valor << "' al '" << claveFin.Valor << "'";
		cout << endl << endl;

		list<BKDRegistro*> resultadoRango;
		arbol->BuscarPorRango(claveInicio, claveFin, resultadoRango);

		cout << "Cantidad de registros encontrados: " << resultadoRango.size() << endl;

		if (resultadoRango.size() > 0)
		{
			cout << "Registros: ";

			for (list<BKDRegistro*>::iterator it = resultadoRango.begin(); it != resultadoRango.end(); it++)
			{
				cout << "{" << (*it)->ToString() << "} ";
				delete *it;
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


	int PruebaArbolDisco()
	{
		string filePath = "TreeTest.uba";
		int tamanioBloque = 512;

		IndiceDatos* indice = IndiceDatos::Crear(filePath, tamanioBloque);


		cout << endl;
		cout << "======================== Comenzando pruebas de Arbol en Disco ... =============";
		cout << endl << endl;

		cout << "Archivo: \"" << filePath << "\"" << endl << endl;

		cout << endl;
		cout << "======================== Fin pruebas de Arbol en Disco ========================";
		cout << endl << endl;

		delete indice;

		cout << ("Pulse enter para terminar.");
		cout << endl << endl;

		getchar();

		return 0;
	}





}



