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
	std::list<BKDClave*> m_claves;
	std::list<int> m_hijos;

public:
	virtual ~BKDNodoInterno();

	BKDNodoInterno(BKDManager* manager, int nroNodo, int capacidad, int nivel);
	BKDNodoInterno(const BKDNodoInterno& ref);

	virtual bool HayOverflow();
	virtual bool HayUnderflow();

	virtual bool BuscarReg(const BKDClave& clave, BKDRegistro** registro);
	virtual bool BuscarRango(const BKDClave& claveInicio, const BKDClave& claveFin, std::list<BKDRegistro*>& resultado);

	virtual bool InsertarReg(const BKDRegistro& registro, bool& overflow);
	virtual bool ModificarReg(const BKDRegistro& registro);
	virtual bool EliminarReg(const BKDClave& clave);

	//Resuelve un overflow en el nodo actual. Devuelve el nuevo hermano creado, y la clave a promover por referencia
	//Ante un error, devuelve NULL como retorno
	virtual BKDNodo* ResolverOverflow(BKDClave** clavePromovida);

	//Recibe un puntero a nodo hoja, el cual será modificado para ser un clon del nodo actual
	virtual bool ClonarNodo(BKDNodo* nodo, bool clonarNroNodo);


	//Metodo para crear una nueva raiz a partir de una vieja. SOLO USAR EN OVERFLOW DE RAIZ!
	static BKDNodoInterno* PromoverRaiz(BKDManager* manager, BKDNodo* viejaRaiz, BKDClave* clavePromovida, int hijoIzquierdo, int hijoDerecho);


	//PARA DEBUG
	virtual void DebugPrint(int nivel);
};

#endif /* BKDNODOINTERNO_H_ */
