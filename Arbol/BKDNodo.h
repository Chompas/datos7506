/*
 * BKDNodo.h
 *
 *  Created on: May 27, 2012
 *      Author: vdiego
 */

#ifndef BKDNODO_H_
#define BKDNODO_H_

#include <list>
#include "Definiciones.h"
#include "BKDManager.h"
class BKDManager; //forward declaration del manager


//Representa un nodo genérico (hoja o interno)
class BKDNodo
{
protected:
	int m_nro_nodo;
	int m_nivel;
	unsigned int m_capacidad;
	BKDManager* m_manager;

public:
	int GetNumeroNodo();
	int GetNivel();
	int GetCapacidad();
	bool EsNodoHoja();

	virtual bool HayOverflow() = 0;
	virtual bool HayUnderflow() = 0;

	virtual bool BuscarReg(const TClave& clave, TRegistro& registro) = 0;
	virtual bool BuscarRango(const TClave& claveInicio, const TClave& claveFin, std::list<TRegistro>& resultado) = 0;

	virtual bool InsertarReg(const TRegistro& registro, bool& overflow) = 0;
	virtual bool ModificarReg(const TRegistro& registro) = 0;
	virtual bool EliminarReg(const TClave& clave) = 0;

	//Resuelve un overflow en el nodo actual. Devuelve el nuevo hermano creado, y la clave a promover por referencia
	//Ante un error, devuelve NULL como retorno
	virtual BKDNodo* ResolverOverflow(TClave& clavePromovida) = 0;

	//Recibe un puntero a nodo, el cual será modificado para ser un clon del nodo actual
	virtual bool ClonarNodo(BKDNodo* nodo, bool clonarNroNodo) = 0;


	//PARA DEBUG
	//imprime el nodo solo si es del nivel especificado. Si hay hijos los llama en cascada.
	virtual void DebugPrint(int nivel) = 0;
};

#endif /* BKDNODO_H_ */
