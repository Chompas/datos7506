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




bool BKDNodoHojaDisco::HayOverflow()
{
	unsigned int tamAcum = 0;
	BKDManagerDisco* md = (BKDManagerDisco*)this->m_manager;

	//acumulo reg siguiente hoja + registros datos

	RegistroDeLongitudVariable* regSiguienteHoja = new RegistroDeLongitudVariable((char*)(&this->m_siguienteHoja), sizeof(int));
	tamAcum += md->CalcularEspacioAOcupar(regSiguienteHoja);
	delete regSiguienteHoja;
	regSiguienteHoja = NULL;

	if (tamAcum > this->m_capacidad)
		return true;


	for (RegsIterator ri = this->m_registros.begin(); ri != this->m_registros.end(); ri++)
	{
		Buffer buff;
		(*ri)->serializar(&buff, 0);

		int tamDatosReg = 0;
		char* datosReg = buff.getStream(tamDatosReg);

		RegistroDeLongitudVariable* regV = new RegistroDeLongitudVariable(datosReg, tamDatosReg);
		delete[] datosReg;

		tamAcum += md->CalcularEspacioAOcupar(regV);
		delete regV;

		if (tamAcum > this->m_capacidad)
			return true;
	}

	return false;
}

bool BKDNodoHojaDisco::HayUnderflow()
{
	//si soy hoja y raiz, no tengo un minimo de registros, siempre puedo eliminar.
	if (this->m_nivel == 0)
		return false;

	unsigned int tamAcum = 0;
	BKDManagerDisco* md = (BKDManagerDisco*)this->m_manager;

	//acumulo reg siguiente hoja + registros datos

	RegistroDeLongitudVariable* regSiguienteHoja = new RegistroDeLongitudVariable((char*)(&this->m_siguienteHoja), sizeof(int));
	tamAcum += md->CalcularEspacioAOcupar(regSiguienteHoja);
	delete regSiguienteHoja;
	regSiguienteHoja = NULL;


	for (RegsIterator ri = this->m_registros.begin(); ri != this->m_registros.end(); ri++)
	{
		Buffer buff;
		(*ri)->serializar(&buff, 0);

		int tamDatosReg = 0;
		char* datosReg = buff.getStream(tamDatosReg);

		RegistroDeLongitudVariable* regV = new RegistroDeLongitudVariable(datosReg, tamDatosReg);
		delete[] datosReg;

		tamAcum += md->CalcularEspacioAOcupar(regV);
		delete regV;
	}

	//tengo que asegurar tener al menos capacidad / 2 registros.
	return (tamAcum < (this->m_capacidad / 2));

}

//Indica la cantidad de registros que se deberían mover del nodo en overflow al nuevo hermano derecho
int BKDNodoHojaDisco::CantidadAMover()
{
	//recorro la lista de registros mientras la cant acumulada + nuevo dato sea menor a capacidad / 2
	//cuando termino, corte porque tengo el maximo espacio ocupado menor a capacidad / 2 en el nodo original,
	//y el resto lo voy a mover al nuevo nodo

	int cant = 0;
	unsigned int tamAcum = 0;
	BKDManagerDisco* md = (BKDManagerDisco*)this->m_manager;

	//primero sumo al acumulado el espacio ocupado por el puntero a la hoja siguiente
	RegistroDeLongitudVariable* regSiguienteHoja = new RegistroDeLongitudVariable((char*)(&this->m_siguienteHoja), sizeof(int));
	tamAcum += md->CalcularEspacioAOcupar(regSiguienteHoja);
	delete regSiguienteHoja;
	regSiguienteHoja = NULL;

	if (tamAcum > (this->m_capacidad / 2))
		cant++;

	for (RegsIterator ri = this->m_registros.begin(); ri != this->m_registros.end(); ri++)
	{
		Buffer buff;
		(*ri)->serializar(&buff, 0);

		int tamDatosReg = 0;
		char* datosReg = buff.getStream(tamDatosReg);

		RegistroDeLongitudVariable* regV = new RegistroDeLongitudVariable(datosReg, tamDatosReg);
		delete[] datosReg;

		tamAcum += md->CalcularEspacioAOcupar(regV);
		delete regV;

		if (tamAcum > (this->m_capacidad / 2))
			cant++;
	}

	return cant;
}



bool BKDNodoHojaDisco::EscribirEnBloque(Bloque* bloque)
{
	Utils::LogDebug(Utils::dbgSS << "Intentando escribir nodo Hoja a bloque en memoria...");

	if (bloque == NULL)
	{
		UT::LogError(UT::errSS << "Error al intentar escribir el nodo " << this->m_nro_nodo << ": bloque NULL");
		return false;
	}

	//grabo el primer registro, que es el puntero a la siguiente hoja.

	//**RegistroDeLongitudFija* regSiguienteHoja = new RegistroDeLongitudFija((char*)(&this->m_siguienteHoja), sizeof(int), sizeof(int));
	RegistroDeLongitudVariable* regSiguienteHoja = new RegistroDeLongitudVariable((char*)(&this->m_siguienteHoja), sizeof(int));

	Utils::LogDebug(Utils::dbgSS << "Intentando grabar informacion de siguiente hoja...");
	if (!bloque->insertarRegistro(regSiguienteHoja))
	{
		UT::LogError(UT::errSS << "Error al intentar escribir nodo hoja "
							   << this->m_nro_nodo
							   << " al bloque: No se pudo escribir el campo 'siguiente hoja'.");
		delete regSiguienteHoja;
		return false;
	}
	else
		Utils::LogDebug(Utils::dbgSS << "Se guardo correctamente el campo 'siguiente hoja'");

	delete regSiguienteHoja;
	regSiguienteHoja = NULL;

	//grabo cada uno de los registros de datos

	Utils::LogDebug(Utils::dbgSS << "Guardando registros de datos al bloque... (cantidad de registros: "
								 << this->m_registros.size() << ")" );

	RegsIterator it = this->m_registros.begin();

	for(int idx = 0; it != this->m_registros.end(); it++, idx++)
	{
		Buffer buff;

		Utils::LogDebug(Utils::dbgSS << "Serializando registro... (posicion: " << idx << ")" );
		if ((*it)->serializar(&buff, 0) != 0)
		{
			UT::LogError(UT::errSS << "Error al intentar escribir nodo hoja " << this->m_nro_nodo
								   << " al bloque: No se pudo serializar el registro de datos en la posicion "
								   << idx);
			return false;
		}
		else
			Utils::LogDebug(Utils::dbgSS << "Registro serializado OK.");

		int tamDatosReg = 0;
		char* datosReg = buff.getStream(tamDatosReg);
		RegistroDeLongitudVariable* regV = new RegistroDeLongitudVariable(datosReg, tamDatosReg);
		delete[] datosReg;

		Utils::LogDebug(Utils::dbgSS << "Intentando insertar datos del registro en el bloque...");
		if (!bloque->entra(regV))
		{
			UT::LogError(UT::errSS << "Error al intentar escribir nodo hoja "
								   << this->m_nro_nodo
								   << " al bloque: espacio insuficiente para insertar registro de datos en la posicion "
								   << idx);
			delete regV;
			return false;
		}

		if (!bloque->insertarRegistro(regV))
		{
			UT::LogError(UT::errSS << "Error al intentar escribir nodo hoja "
								   << this->m_nro_nodo
								   << " al bloque: No se pudo escribir el registro de datos en la posicion "
								   << idx);
			delete regV;
			return false;
		}
		else
			Utils::LogDebug(Utils::dbgSS << "Registro insertado correctamente en el bloque.");

		delete regV;
	}

	Utils::LogDebug(Utils::dbgSS << "Se escribio correctamente el nodo Hoja al bloque en memoria.");

	return true;
}


bool BKDNodoHojaDisco::LeerDeBloque(Bloque* bloque)
{
	Utils::LogDebug(Utils::dbgSS << "Intentando Leer nodo Hoja desde bloque en memoria...");

	if (bloque == NULL)
	{
		UT::LogError(UT::errSS << "Error al intentar Leer el nodo Hoja " << this->m_nro_nodo << ": bloque NULL");
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

	Utils::LogDebug(Utils::dbgSS << "Intentando obtener informacion de siguiente hoja...");
	//Leo el primer registro, que es el puntero a la siguiente hoja.
	Registro* regSiguienteHoja = bloque->obtenerRegistro(regIdx);

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

	Utils::LogDebug(Utils::dbgSS << "Leyendo registros de datos al bloque... (cantidad de registros: "
								 << cantRegs << ")" );


	for (int i = 0; i < cantRegs; i++, regIdx++)
	{
		Utils::LogDebug(Utils::dbgSS << "Leyendo registro de datos numero " << regIdx << "...");
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
		delete[] dato;
		dato = NULL;

		if (regBKD->hidratar(&buffDato, 0) != 0)
		{
			UT::LogError(UT::errSS << "Error al leer el nodo " << this->m_nro_nodo
								   << ", registro " << regIdx
								   << ": No se pudo hidratar el registro");

			delete regBKD;
			return false;
		}

		this->m_registros.push_back(regBKD);

		Utils::LogDebug(Utils::dbgSS << "Registro leido correctamente del bloque.");
	}

	Utils::LogDebug(Utils::dbgSS << "Se Leyo correctamente el nodo Hoja desde el bloque en memoria.");

	return true;
}


