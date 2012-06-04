/*
 * BKDNodoHoja.h
 *
 *  Created on: May 27, 2012
 *      Author: vdiego
 */

#ifndef BKDNODOHOJA_H_
#define BKDNODOHOJA_H_

#include "BKDNodo.h"
#include <list>

//Representa un nodo hoja
class BKDNodoHoja : public BKDNodo
{
protected:
	std::list<BKDRegistro*> m_registros;
	int m_siguienteHoja;

public:
	virtual ~BKDNodoHoja();

	BKDNodoHoja(BKDManager* manager,int nroNodo, int capacidad, int nivel);
	BKDNodoHoja(const BKDNodoHoja& ref);

	int GetSiguienteHoja();

	virtual bool HayOverflow();
	virtual bool HayUnderflow();
	virtual int CantidadAMover(); //Indica la cantidad de registros que se deberían mover del nodo en overflow al nuevo hermano derecho

	virtual bool BuscarReg(const BKDClaveMultiple& clave, BKDRegistro** registro);
	virtual bool BuscarRango(const BKDClaveMultiple& claveInicio, const BKDClaveMultiple& claveFin, std::list<BKDRegistro*>& resultado);

	virtual bool InsertarReg(const BKDRegistro& registro, bool& overflow);
	virtual bool ModificarReg(const BKDRegistro& registro);
	virtual bool EliminarReg(const BKDClave& clave);

	//Resuelve un overflow en el nodo actual. Devuelve el nuevo hermano creado, y la clave a promover por referencia
	//Ante un error, devuelve NULL como retorno
	virtual BKDNodo* ResolverOverflow(BKDClave** clavePromovida);

	//Recibe un puntero a nodo hoja, el cual será modificado para ser un clon del nodo actual
	virtual bool ClonarNodo(BKDNodo* nodo, bool clonarNroNodo);


	//PARA DEBUG
	virtual void DebugPrint(int nivel);
};



#endif /* BKDNODOHOJA_H_ */
