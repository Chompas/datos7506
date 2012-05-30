/*
 * BKDManager.h
 *
 *  Created on: May 28, 2012
 *      Author: vdiego
 */

#ifndef BKDMANAGER_H_
#define BKDMANAGER_H_

#include "BKDNodo.h"
class BKDNodo; //forward declaration del nodo

//abstracción para manejo de almacenamiento de nodos
class BKDManager
{
protected:
	int m_raiz_id;
	int m_capacidad_hoja;
	int m_capacidad_interno;

public:
	//!!Importante: Es responsabilidad del caller liberar la memoria devuelta
	virtual BKDNodo* GetNodoRaiz() = 0;
	virtual BKDNodo* GetNodoPrimeraHoja() = 0;
	virtual BKDNodo* GetNodo(int nroNodo) = 0;

	virtual BKDNodo* AgregarNodo(int nivel) = 0;
	virtual bool GuardarNodo(BKDNodo* nodo) = 0;
	virtual bool BorrarNodo(int nroNodo) = 0;

};


#endif /* BKDMANAGER_H_ */
