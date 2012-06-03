/*
 * BKDNodoHojaDisco.cpp
 *
 *  Created on: May 31, 2012
 *      Author: vdiego
 */

#include "BKDNodoHojaDisco.h"

#include "BKDManagerDisco.h"
#include "../Comun/Utils.h"
//**#include "../Disco/RegistroDeLongitudFija.h"
#include "../Disco/RegistroDeLongitudVariable.h"


namespace UT = Utils;
typedef list<BKDRegistro*>::iterator RegsIterator;


BKDNodoHojaDisco::BKDNodoHojaDisco(BKDManager* manager,int nroNodo, int capacidad, int nivel)
				: BKDNodoHoja(manager, nroNodo, capacidad, nivel) { }


bool BKDNodoHojaDisco::EscribirEnBloque(Bloque* bloque)
{
	if (bloque == NULL)
	{
		UT::LogError(UT::errSS << "Error al intentar escribir el nodo " << this->m_nro_nodo << ": bloque NULL");
		return false;
	}

	//grabo el primer registro, que es el puntero a la siguiente hoja.

	//**RegistroDeLongitudFija* regSiguienteHoja = new RegistroDeLongitudFija((char*)(&this->m_siguienteHoja), sizeof(int), sizeof(int));
	RegistroDeLongitudVariable* regSiguienteHoja = new RegistroDeLongitudVariable((char*)(&this->m_siguienteHoja), sizeof(int));

	if (!bloque->insertarRegistro(regSiguienteHoja))
	{
		UT::LogError(UT::errSS << "Error al intentar escribir nodo hoja "
							   << this->m_nro_nodo
							   << " al bloque: No se pudo escribir el campo 'siguiente hoja'.");
		delete regSiguienteHoja;
		return false;
	}

	delete regSiguienteHoja;

	//grabo cada uno de los registros de datos

	RegsIterator it = this->m_registros.begin();

	for(int idx = 0; it != this->m_registros.end(); it++, idx++)
	{
		Buffer buff;

		if ((*it)->serializar(&buff, 0) != 0)
		{
			UT::LogError(UT::errSS << "Error al intentar escribir nodo hoja " << this->m_nro_nodo
								   << " al bloque: No se pudo serializar el registro de datos en la posicion "
								   << idx);
			return false;
		}


		int tamDatosReg = 0;
		char* datosReg = buff.getStream(tamDatosReg);
		RegistroDeLongitudVariable* regV = new RegistroDeLongitudVariable(datosReg, tamDatosReg);
		delete[] datosReg;

		if (!bloque->insertarRegistro(regV))
		{
			UT::LogError(UT::errSS << "Error al intentar escribir nodo hoja "
								   << this->m_nro_nodo
								   << " al bloque: No se pudo escribir el registro de datos en la posicion "
								   << idx);
			delete regV;
			return false;
		}

		delete regV;
	}

	return true;
}


bool BKDNodoHojaDisco::LeerDeBloque(Bloque* bloque)
{
	if (bloque == NULL)
	{
		UT::LogError(UT::errSS << "Error al intentar Leer el nodo " << this->m_nro_nodo << ": bloque NULL");
		return false;
	}

	int cantRegs = bloque->obtenerCantidadRegistros();

	//Ajusto por registro de control
	if (bloque->existeRegistroDeControl())
		cantRegs--;

	if (cantRegs < 1)
	{
		UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": falta puntero a siguiente hoja");
		return false;
	}

	int regIdx = 1; //los registros van del 1 al N
	//Leo el primer registro, que es el puntero a la siguiente hoja.
	Registro* regSiguienteHoja = bloque->obtenerRegistro(1);

	if (regSiguienteHoja == NULL)
	{
		UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": registro de siguiente hoja ES NULL");
		return false;
	}

	int tamSigHoja = 0;
	char* sigHoja = regSiguienteHoja->getDato(tamSigHoja);
	delete regSiguienteHoja;
	regSiguienteHoja = NULL;

	if (sigHoja == NULL)
	{
		UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": siguiente hoja es NULL");
		return false;
	}

	if (tamSigHoja != sizeof(int))
	{
		UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": tamanio de siguiente hoja invalido");
		delete[] sigHoja;
		return false;
	}

	this->m_siguienteHoja = *(int*)(sigHoja);
	delete[] sigHoja;
	sigHoja = NULL;

	if (this->m_siguienteHoja < -1)
	{
		UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": informacion de siguiente hoja corrupta");
		return false;
	}

	cantRegs--; //lo decremento para utilizarlo como contador de registros de datos
	regIdx++;

	BKDManagerDisco* manDisco = (BKDManagerDisco*)this->m_manager;

	for (int i = 0; i < cantRegs; i++, regIdx++)
	{
		Registro* regDato = bloque->obtenerRegistro(regIdx);

		if (regDato == NULL)
		{
			UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": registro de siguiente hoja ES NULL");
			return false;
		}

		int tamDato = 0;
		char* dato = regDato->getDato(tamDato);
		delete regDato;
		regDato = NULL;

		if (dato == NULL)
		{
			UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ", registro " << regIdx << ": el dato es NULL");
			return false;
		}

		BKDRegistro* regBKD = manDisco->InstanciarRegistro();

		if (regBKD == NULL)
		{
			UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo
								   << ", registro " << regIdx
								   << ": No se pudo instanciar el registro");
			delete[] dato;
			return false;
		}

		Buffer buffDato = Buffer(dato, tamDato);

		if (regBKD->hidratar(&buffDato, 0) != 0)
		{
			UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo
								   << ", registro " << regIdx
								   << ": No se pudo hidratar el registro");
			delete[] dato;
			delete regBKD;
			return false;
		}

		this->m_registros.push_back(regBKD);

		delete[] dato;
		dato = NULL;
	}

	return true;
}


