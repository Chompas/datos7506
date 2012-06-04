/*
 * BKDNodoInternoDisco.cpp
 *
 *  Created on: May 31, 2012
 *      Author: vdiego
 */

#include "BKDNodoInternoDisco.h"

#include "BKDManagerDisco.h"
#include "../Comun/Utils.h"
#include "../Disco/RegistroDeLongitudVariable.h"

namespace UT = Utils;

typedef list<BKDClaveMultiple*>::iterator ClavesIterator;
typedef list<int>::iterator HijosIterator;



BKDNodoInternoDisco::BKDNodoInternoDisco(BKDManager* manager,int nroNodo, int capacidad, int nivel)
				: BKDNodoInterno(manager, nroNodo, capacidad, nivel) { }





bool BKDNodoInternoDisco::HayOverflow()
{
	unsigned int tamAcum = 0;
	BKDManagerDisco* md = (BKDManagerDisco*)this->m_manager;

	//acumulo registro cantidad hijos + registros hijos + registro cantidad Claves + registros claves

	int cantHijos = this->m_hijos.size();
	RegistroDeLongitudVariable* regCantHijos = new RegistroDeLongitudVariable((char*)(&cantHijos), sizeof(int));
	tamAcum += md->CalcularEspacioAOcupar(regCantHijos);
	delete regCantHijos;
	regCantHijos = NULL;

	if (tamAcum > this->m_capacidad)
		return true;

	for (HijosIterator hi = this->m_hijos.begin(); hi != this->m_hijos.end(); hi++)
	{
		RegistroDeLongitudVariable* regHijo = new RegistroDeLongitudVariable((char*)(&(*hi)), sizeof(int));
		tamAcum += md->CalcularEspacioAOcupar(regHijo);
		delete regHijo;
		regHijo = NULL;

		if (tamAcum > this->m_capacidad)
			return true;
	}

	int cantClaves = this->m_claves.size();
	RegistroDeLongitudVariable* regCantClaves = new RegistroDeLongitudVariable((char*)(&cantClaves), sizeof(int));
	tamAcum += md->CalcularEspacioAOcupar(regCantClaves);
	delete regCantClaves;
	regCantClaves = NULL;

	if (tamAcum > this->m_capacidad)
		return true;

	for (ClavesIterator ci = this->m_claves.begin(); ci != this->m_claves.end(); ci++)
	{
		Buffer buff;
		(*ci)->serializar(&buff, 0);

		int tamDatosClave = 0;
		char* datosClave = buff.getStream(tamDatosClave);

		RegistroDeLongitudVariable* regV = new RegistroDeLongitudVariable(datosClave, tamDatosClave);
		delete[] datosClave;

		tamAcum += md->CalcularEspacioAOcupar(regV);
		delete regV;

		if (tamAcum > this->m_capacidad)
			return true;
	}

	return false;
}

bool BKDNodoInternoDisco::HayUnderflow()
{
	unsigned int tamAcum = 0;
	BKDManagerDisco* md = (BKDManagerDisco*)this->m_manager;

	//acumulo reg cant hijos + registros hijos + reg cant claves + regs claves

	int cantHijos = this->m_hijos.size();
	RegistroDeLongitudVariable* regCantHijos = new RegistroDeLongitudVariable((char*)(&cantHijos), sizeof(int));
	tamAcum += md->CalcularEspacioAOcupar(regCantHijos);
	delete regCantHijos;
	regCantHijos = NULL;

	for (HijosIterator hi = this->m_hijos.begin(); hi != this->m_hijos.end(); hi++)
	{
		RegistroDeLongitudVariable* regHijo = new RegistroDeLongitudVariable((char*)(&(*hi)), sizeof(int));
		tamAcum += md->CalcularEspacioAOcupar(regHijo);
		delete regHijo;
		regHijo = NULL;
	}

	int cantClaves = this->m_claves.size();
	RegistroDeLongitudVariable* regCantClaves = new RegistroDeLongitudVariable((char*)(&cantClaves), sizeof(int));
	tamAcum += md->CalcularEspacioAOcupar(regCantClaves);
	delete regCantClaves;
	regCantClaves = NULL;


	for (ClavesIterator ci = this->m_claves.begin(); ci != this->m_claves.end(); ci++)
	{
		Buffer buff;
		(*ci)->serializar(&buff, 0);

		int tamDatosClave = 0;
		char* datosClave = buff.getStream(tamDatosClave);

		RegistroDeLongitudVariable* regV = new RegistroDeLongitudVariable(datosClave, tamDatosClave);
		delete[] datosClave;

		tamAcum += md->CalcularEspacioAOcupar(regV);
		delete regV;
	}

	//tengo que asegurar tener al menos capacidad / 2 registros.
		return (tamAcum < (this->m_capacidad / 2));
}

//Indica la cantidad de registros que se deberían mover del nodo en overflow al nuevo hermano derecho
int BKDNodoInternoDisco::CantidadAMover()
{
	//recorro la lista de claves mientras la cant acumulada + cada par(clave, hijoIzq) sea menor a capacidad / 2
	//cuando termino, corte porque tengo el maximo espacio ocupado menor a capacidad / 2 en el nodo original,
	//y el resto lo voy a mover al nuevo nodo. (La ultima clave copiada en realidad no va a ocupar espacio porque
	//va a ser la clave promovida al nivel superior.

	int cant = 0;
	unsigned int tamAcum = 0;
	BKDManagerDisco* md = (BKDManagerDisco*)this->m_manager;

	//primero sumo al acumulado el espacio ocupado por las cantidades de hijos y claves.
	//uso cualquier dato int para calcular el tamanio final del registro, y multiplico por 2...
	RegistroDeLongitudVariable* regCant = new RegistroDeLongitudVariable((char*)(&cant), sizeof(int));
	tamAcum += 2 * (md->CalcularEspacioAOcupar(regCant));
	delete regCant;
	regCant = NULL;


	//ahora voy recorriendo de a pares (clave, hijoIzq) y cuento cuantos se pasan del limite de capacidad / 2

	HijosIterator hi = this->m_hijos.begin();

	for (ClavesIterator ci = this->m_claves.begin(); ci != this->m_claves.end(); ci++, hi++)
	{
		Buffer buff;
		(*ci)->serializar(&buff, 0);

		int tamDatosClave = 0;
		char* datosClave = buff.getStream(tamDatosClave);

		RegistroDeLongitudVariable* regV = new RegistroDeLongitudVariable(datosClave, tamDatosClave);
		delete[] datosClave;

		tamAcum += md->CalcularEspacioAOcupar(regV);
		delete regV;

		RegistroDeLongitudVariable* regHijo = new RegistroDeLongitudVariable((char*)(&(*hi)), sizeof(int));
		tamAcum += md->CalcularEspacioAOcupar(regHijo);
		delete regHijo;
		regHijo = NULL;

		if (tamAcum > (this->m_capacidad / 2))
			cant++;
	}

	return cant;
}


bool BKDNodoInternoDisco::EscribirEnBloque(Bloque* bloque)
{
	Utils::LogDebug(Utils::dbgSS << "Intentando escribir nodo Interno a bloque en memoria...");

	if (bloque == NULL)
	{
		UT::LogError(UT::errSS << "Error al intentar escribir el nodo " << this->m_nro_nodo << ": bloque NULL");
		return false;
	}

	//grabo primero todos los punteros a hijos, y luego las claves

	int cantHijos = this->m_hijos.size();
	Utils::LogDebug(Utils::dbgSS << "Guardando registros de punteros a hijos... (cantidad de hijos: "
								 << cantHijos << ")" );

	//guardo la cantidad de hijos, y luego los punteros a hijos

	RegistroDeLongitudVariable* regCantHijos = new RegistroDeLongitudVariable((char*)(&cantHijos), sizeof(int));

	Utils::LogDebug(Utils::dbgSS << "Intentando grabar cantidad de hijos...");
	if (!bloque->insertarRegistro(regCantHijos))
	{
		UT::LogError(UT::errSS << "Error al intentar escribir nodo Interno "
							   << this->m_nro_nodo
							   << " al bloque: No se pudo escribir la cantidad de hijos.");
		delete regCantHijos;
		return false;
	}
	else
		Utils::LogDebug(Utils::dbgSS << "Se guardo correctamente la cantidad de hijos");

	delete regCantHijos;
	regCantHijos = NULL;

	//ahora guardo los hijos
	HijosIterator hi = this->m_hijos.begin();

	for(int idx = 0; hi != this->m_hijos.end(); hi++, idx++)
	{
		int hijo = *hi;
		RegistroDeLongitudVariable* regV = new RegistroDeLongitudVariable((char*)&hijo, sizeof(int));

		Utils::LogDebug(Utils::dbgSS << "Intentando insertar registro de puntero a hijo... (posicion: " << idx << ")" );

		if (!bloque->entra(regV))
		{
			UT::LogError(UT::errSS << "Error al intentar escribir nodo Interno "
								   << this->m_nro_nodo
								   << " al bloque: espacio insuficiente para escribir el registro de puntero a hijo en la posicion "
								   << idx);
			delete regV;
			return false;
		}

		if (!bloque->insertarRegistro(regV))
		{
			UT::LogError(UT::errSS << "Error al intentar escribir nodo Interno "
								   << this->m_nro_nodo
								   << " al bloque: No se pudo escribir el registro de puntero a hijo en la posicion "
								   << idx);
			delete regV;
			return false;
		}
		else
			Utils::LogDebug(Utils::dbgSS << "Registro insertado correctamente en el bloque.");

		delete regV;
	}

	Utils::LogDebug(Utils::dbgSS << "registros de Hijos guardados correctamente.");

	//Ahora guardo las claves con su cantidad
	int cantClaves = this->m_claves.size();
	Utils::LogDebug(Utils::dbgSS << "Guardando registros de claves ... (cantidad de claves: "
									 << cantClaves << ")" );

	RegistroDeLongitudVariable* regCantClaves = new RegistroDeLongitudVariable((char*)(&cantClaves), sizeof(int));

	Utils::LogDebug(Utils::dbgSS << "Intentando grabar cantidad de claves...");
	if (!bloque->insertarRegistro(regCantClaves))
	{
		UT::LogError(UT::errSS << "Error al intentar escribir nodo Interno "
							   << this->m_nro_nodo
							   << " al bloque: No se pudo escribir la cantidad de claves.");
		delete regCantClaves;
		return false;
	}
	else
		Utils::LogDebug(Utils::dbgSS << "Se guardo correctamente la cantidad de claves");

	delete regCantClaves;
	regCantClaves = NULL;

	//ahora guardo las claves
	ClavesIterator ci = this->m_claves.begin();

	for(int idx = 0; ci != this->m_claves.end(); ci++, idx++)
	{
		Buffer buff;

		Utils::LogDebug(Utils::dbgSS << "Serializando clave... (posicion: " << idx << ")" );
		if ((*ci)->serializar(&buff, 0) != 0)
		{
			UT::LogError(UT::errSS << "Error al intentar escribir nodo Interno " << this->m_nro_nodo
								   << " al bloque: No se pudo serializar el registro de clave en la posicion "
								   << idx);
			return false;
		}
		else
			Utils::LogDebug(Utils::dbgSS << "Registro serializado OK.");

		int tamDatosReg = 0;
		char* datosReg = buff.getStream(tamDatosReg);
		RegistroDeLongitudVariable* regV = new RegistroDeLongitudVariable(datosReg, tamDatosReg);
		delete[] datosReg;

		Utils::LogDebug(Utils::dbgSS << "Intentando insertar datos del registro de clave en el bloque...");

		if (!bloque->entra(regV))
		{
			UT::LogError(UT::errSS << "Error al intentar escribir nodo Interno "
								   << this->m_nro_nodo
								   << " al bloque: espacio insuficiente para escribir el registro de clave en la posicion "
								   << idx);
			delete regV;
			return false;
		}

		if (!bloque->insertarRegistro(regV))
		{
			UT::LogError(UT::errSS << "Error al intentar escribir nodo Interno "
								   << this->m_nro_nodo
								   << " al bloque: No se pudo escribir el registro de clave en la posicion "
								   << idx);
			delete regV;
			return false;
		}
		else
			Utils::LogDebug(Utils::dbgSS << "Registro insertado correctamente en el bloque.");

		delete regV;
	}

	Utils::LogDebug(Utils::dbgSS << "Se escribio correctamente el nodo al bloque en memoria.");

	return true;
}

bool BKDNodoInternoDisco::LeerDeBloque(Bloque* bloque)
{
	Utils::LogDebug(Utils::dbgSS << "Intentando Leer nodo Interno desde bloque en memoria...");

	if (bloque == NULL)
	{
		UT::LogError(UT::errSS << "Error al intentar Leer el nodo Interno " << this->m_nro_nodo << ": bloque NULL");
		return false;
	}

	int cantRegs = bloque->obtenerCantidadRegistros();

	//Ajusto por registro de control
	if (bloque->existeRegistroDeControl())
		cantRegs--;

	if (cantRegs < 2)
	{
		UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": falta la cantidad de hijos y claves");
		return false;
	}


	int regIdx = 1; //los registros van del 1 al N

	//La estructura es #hijos -> {hijos} -> #claves -> {claves}

	Utils::LogDebug(Utils::dbgSS << "Leyendo cantidad de hijos " << regIdx << "...");
	Registro* regCantHijos = bloque->obtenerRegistro(regIdx);

	if (regCantHijos == NULL)
	{
		UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": registro de Cantidad de hijos ES NULL");
		return false;
	}

	int tamCantHijos = 0;
	char* bfCantHijos = regCantHijos->getDato(tamCantHijos);
	delete regCantHijos;
	regCantHijos = NULL;

	if (bfCantHijos == NULL)
	{
		UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": Cantidad de hijos es NULL");
		return false;
	}

	if (tamCantHijos != sizeof(int))
	{
		UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": tamanio de cantidad de hijos invalido");
		delete[] bfCantHijos;
		return false;
	}

	int cantHijos = *(int*)(bfCantHijos);
	delete[] bfCantHijos;
	bfCantHijos = NULL;

	if (cantHijos < 0)
	{
		UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": informacion de cantidad de hijos corrupta");
		return false;
	}

	Utils::LogDebug(Utils::dbgSS << "Leida Cantidad de hijos: " << cantHijos);

	regIdx++;


	BKDManagerDisco* manDisco = (BKDManagerDisco*)this->m_manager;
	//ahora que tengo la cantidad de hijos los leo y cargo a la lista

	for (int i = 0; i < cantHijos; i++, regIdx++)
	{
		Utils::LogDebug(Utils::dbgSS << "Leyendo registro de hijos numero " << regIdx << "...");
		Registro* regHijo = bloque->obtenerRegistro(regIdx);

		if (regHijo == NULL)
		{
			UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": registro de hijo ES NULL");
			return false;
		}

		int tamHijo = 0;
		char* bfHijo = regHijo->getDato(tamHijo);
		delete regHijo;
		regHijo = NULL;

		if (bfHijo == NULL)
		{
			UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ", registro " << regIdx << ": el dato es NULL");
			return false;
		}

		int hijo = *(int*)bfHijo;
		delete[] bfHijo;
		bfHijo = NULL;

		this->m_hijos.push_back(hijo);
		Utils::LogDebug(Utils::dbgSS << "Registro Leido OK");
	}

	Utils::LogDebug(Utils::dbgSS << "Leidos los registros de hijos. Leyendo cantidad de claves...");

	//Ahora viene la cantidad de claves y la lista de claves

	Registro* regCantClaves = bloque->obtenerRegistro(regIdx);

	if (regCantClaves == NULL)
	{
		UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": registro de Cantidad de claves ES NULL");
		return false;
	}

	int tamCantClaves = 0;
	char* bfCantClaves = regCantClaves->getDato(tamCantClaves);
	delete regCantClaves;
	regCantClaves = NULL;

	if (bfCantClaves == NULL)
	{
		UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": Cantidad de claves es NULL");
		return false;
	}

	if (tamCantClaves != sizeof(int))
	{
		UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": tamanio de cantidad de claves invalido");
		delete[] bfCantClaves;
		return false;
	}

	int cantClaves = *(int*)(bfCantClaves);
	delete[] bfCantClaves;
	bfCantClaves = NULL;

	if (cantClaves < 0)
	{
		UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": informacion de cantidad de claves corrupta");
		return false;
	}


	Utils::LogDebug(Utils::dbgSS << "Leida Cantidad de claves: " << cantClaves);
	regIdx++;

	for (int i = 0; i < cantClaves; i++, regIdx++)
	{
		Utils::LogDebug(Utils::dbgSS << "Leyendo registro de claves numero " << regIdx << "...");
		Registro* regClave = bloque->obtenerRegistro(regIdx);

		if (regClave == NULL)
		{
			UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ": registro de clave ES NULL");
			return false;
		}

		int tamClave = 0;
		char* bfClave = regClave->getDato(tamClave);
		delete regClave;
		regClave = NULL;

		if (bfClave == NULL)
		{
			UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo << ", registro " << regIdx << ": el dato es NULL");
			return false;
		}

		BKDClaveMultiple* clave = manDisco->InstanciarClave();

		if (clave == NULL)
		{
			UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo
								   << ", registro " << regIdx
								   << ": No se pudo instanciar la clave");
			delete[] bfClave;
			return false;
		}

		Buffer buffClave = Buffer(bfClave, tamClave);
		delete[] bfClave;
		bfClave = NULL;

		if (clave->hidratar(&buffClave, 0) != 0)
		{
			UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo
								   << ", registro " << regIdx
								   << ": No se pudo hidratar la clave");
			delete clave;
			return false;
		}

		this->m_claves.push_back(clave);
	}

	Utils::LogDebug(Utils::dbgSS << "Se Leyo correctamente el nodo Hoja desde el bloque en memoria.");

	return true;
}

