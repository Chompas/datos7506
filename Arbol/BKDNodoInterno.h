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
	std::list<BKDClaveMultiple*> m_claves;
	std::list<int> m_hijos;

public:
	virtual ~BKDNodoInterno();

	BKDNodoInterno(BKDManager* manager, int nroNodo, int capacidad, int nivel);
	BKDNodoInterno(const BKDNodoInterno& ref);

	virtual bool HayOverflow();
	virtual bool HayUnderflow();
	virtual int CantidadAMover(); //Indica la cantidad de claves que se deberían mover del nodo en overflow al nuevo hermano derecho

	virtual bool BuscarReg(const BKDClaveMultiple& clave, BKDRegistro** registro, int profundidad);
	virtual bool BuscarRango(const BKDClaveMultiple& claveInicio, const BKDClaveMultiple& claveFin, std::list<BKDRegistro*>& resultado, int profundidad);

	virtual bool InsertarReg(const BKDRegistro& registro, bool& overflow, int profundidad);
	virtual bool ModificarReg(const BKDRegistro& registro, int profundidad);
	virtual bool EliminarReg(const BKDClaveMultiple& clave, int profundidad);

	//Resuelve un overflow en el nodo actual. Devuelve el nuevo hermano creado, y la clave a promover por referencia
	//Ante un error, devuelve NULL como retorno
	virtual BKDNodo* ResolverOverflow(BKDClaveMultiple** clavePromovida);

	//Recibe un puntero a nodo hoja, el cual será modificado para ser un clon del nodo actual
	virtual bool ClonarNodo(BKDNodo* nodo, bool clonarNroNodo);


	//Metodo para crear una nueva raiz a partir de una vieja. SOLO USAR EN OVERFLOW DE RAIZ!
	static BKDNodoInterno* PromoverRaiz(BKDManager* manager, BKDNodo* viejaRaiz, BKDClaveMultiple* clavePromovida, int hijoIzquierdo, int hijoDerecho);


	//PARA DEBUG
	virtual void DebugPrint(int nivel);
};

#endif /* BKDNODOINTERNO_H_ */
