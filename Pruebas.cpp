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
#include "Claves/InstanciadorIncidentes.h"
#include "Claves/ClaveIncidente.h"
#include "Claves/Incidente.h"
#include "Comun/Utils.h"

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

	int PruebaArbolB2()
	{
		string filePath = "PruebaArbolB2.UBA";
		int cap_hoja = 4;
		int cap_int = 3;

		Utils::debugMode = true;

		//BKDArbol* arbol = BKDArbol::CrearEnMemoria(cap_hoja, cap_int);

		BKDArbol* arbol = BKDArbol::CrearEnDisco(filePath, 128, new InstanciadorIncidentes());
		//BKDArbol* arbol = BKDArbol::AbrirDeDisco(filePath, new InstanciadorIncidentes());


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


		int cantRegs = 7;
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
			stringstream ss;

			ss << "Mitre;2011-10-2T12:12:45;no_cierra_puertas;incendio;" << i;


			Incidente reg = Incidente(ss.str());


			if (!arbol->InsertarRegistro(reg))
			{
				cerr << "Error al insertar clave " << reg.formacion << endl;
				delete arbol;
				return -1;
			}

			/*
			Incidente reg2 = Incidente("Belgrano_norte;2010-10-22T12:09:25;motores_al_80;desacoplado;25");

			if (!arbol->InsertarRegistro(reg2))
			{
				cerr << "Error al insertar clave " << reg.formacion << endl;
				delete arbol;
				return -1;
			}

			Incidente reg3 = Incidente("Urquiza;2011-03-15T18:26:09;frena_50;no_enciende;10");

			if (!arbol->InsertarRegistro(reg3))
			{
				cerr << "Error al insertar clave " << reg.formacion << endl;
				delete arbol;
				return -1;
			}

			Incidente reg4 = Incidente("Sarmiento;2012-03-25T19:47:02;frena_80;choque_con_otra_formacion;12");

			if (!arbol->InsertarRegistro(reg4))
			{
				cerr << "Error al insertar clave " << reg.formacion << endl;
				delete arbol;
				return -1;
			}
			*/


		}

		cout << endl;
		cout << "Imprimiendo Arbol B+ ...";
		cout << endl << endl;

		arbol->DebugPrintAll();



		cout << endl;
		cout << "Buscando claves '-2', '0', '7', '99', '7', '666', '1249023'";
		cout << endl << endl;

		int busq[] = {-2, 0, 7, 99, 7, 666, 1249023};

		/*
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
		}*/

		cout << endl;
		cout << "Buscando Incidente Mitre;2011-10-2T12:12:45;no_cierra_puertas;incendio;15";
		cout << endl << endl;

		BKDRegistro* regBusq = NULL;
		Incidente regBus = Incidente("Mitre;2011-10-2T12:12:45;no_cierra_puertas;incendio;15");
		ClaveIncidente claveBus = ClaveIncidente(regBus);
		bool res = arbol->BuscarRegistro(claveBus, &regBusq);

		if (!res)
			cout << " NOPE ";
		else
			cout << " OK ";

		cout << endl;
		cout << endl;



		//ClaveInt claveInicio = ClaveInt(0);
		//ClaveInt claveFin = ClaveInt(500);
		Incidente regInicio = Incidente("Mitre;2011-10-2T12:12:45;no_cierra_puertas;incendio;15");
		Incidente regFin = Incidente("Urquiza;2011-03-15T18:26:09;frena_50;no_enciende;10");
		ClaveIncidente claveInicio = ClaveIncidente(regInicio);
		ClaveIncidente claveFin = ClaveIncidente(regFin);

		cout << endl;
		cout << "Buscando rango de claves '"<< claveInicio.ToString() << "' al '" << claveFin.ToString() << "'";
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



