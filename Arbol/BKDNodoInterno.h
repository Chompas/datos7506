/*
 * BKDNodoInterno.h
 *
 *  Created on: May 27, 2012
 *      Author: vdiego
 */

#ifndef BKDNODOINTERNO_H_
#define BKDNODOINTERNO_H_

#include "BKDNodo.h"
#include <list>

//Representa un nodo interno
class BKDNodoInterno : public BKDNodo
{
protected:
	std::list<TClave> claves;
	std::list<int> hijos;

public:
	BKDNodoInterno(BKDManager* manager, int nroNodo, int capacidad, int nivel);
	BKDNodoInterno(const BKDNodoInterno& ref);

	virtual bool HayOverflow();
	virtual bool HayUnderflow();

	virtual bool BuscarReg(const TClave& clave, TRegistro& registro);
	virtual bool BuscarRango(const TClave& claveInicio, const TClave& claveFin, std::list<TRegistro>& resultado);

	virtual bool InsertarReg(const TRegistro& registro, bool& overflow);
	virtual bool ModificarReg(const TRegistro& registro);
	virtual bool EliminarReg(const TClave& clave);

	//Resuelve un overflow en el nodo actual. Devuelve el nuevo hermano creado, y la clave a promover por referencia
	//Ante un error, devuelve NULL como retorno
	virtual BKDNodo* ResolverOverflow(TClave& clavePromovida);

	//Recibe un puntero a nodo hoja, el cual será modificado para ser un clon del nodo actual
	virtual bool ClonarNodo(BKDNodo* nodo, bool clonarNroNodo);


	//Metodo para crear una nueva raiz a partir de una vieja. SOLO USAR EN OVERFLOW DE RAIZ!
	static BKDNodoInterno* PromoverRaiz(BKDManager* manager, BKDNodo* viejaRaiz, TClave clavePromovida, int hijoIzquierdo, int hijoDerecho);


	//PARA DEBUG
	virtual void DebugPrint(int nivel);
};

#endif /* BKDNODOINTERNO_H_ */
