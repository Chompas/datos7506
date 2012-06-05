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
#include "BKDClaves.h"
#include "BKDRegistro.h"
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
	virtual ~BKDNodo() {}

	int GetNumeroNodo();
	int GetNivel();
	int GetCapacidad();
	bool EsNodoHoja();

	virtual bool HayOverflow() = 0;
	virtual bool HayUnderflow() = 0;
	virtual int CantidadAMover() = 0;//Indica la cantidad de registros que se deberían mover del nodo en overflow al nuevo hermano derecho

	virtual bool BuscarReg(const BKDClaveMultiple& clave, BKDRegistro** registro, int profundidad) = 0;
	virtual bool BuscarRango(const BKDClaveMultiple& claveInicio, const BKDClaveMultiple& claveFin, std::list<BKDRegistro*>& resultado, int profundidad) = 0;

	virtual bool InsertarReg(const BKDRegistro& registro, bool& overflow, int profundidad) = 0;
	virtual bool ModificarReg(const BKDRegistro& registro, int profundidad) = 0;
	virtual bool EliminarReg(const BKDClaveMultiple& clave, int profundidad) = 0;

	//Resuelve un overflow en el nodo actual. Devuelve el nuevo hermano creado, y la clave a promover por referencia
	//Ante un error, devuelve NULL como retorno
	virtual BKDNodo* ResolverOverflow(BKDClaveMultiple** clavePromovida) = 0;

	//Recibe un puntero a nodo, el cual será modificado para ser un clon del nodo actual
	virtual bool ClonarNodo(BKDNodo* nodo, bool clonarNroNodo) = 0;


	//PARA DEBUG
	//imprime el nodo solo si es del nivel especificado. Si hay hijos los llama en cascada.
	virtual void DebugPrint(int nivel) = 0;
};

#endif /* BKDNODO_H_ */
