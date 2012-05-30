/*
 * BKDManagerMemoria.h
 *
 *  Created on: May 28, 2012
 *      Author: vdiego
 */

#ifndef BKDMANAGERMEMORIA_H_
#define BKDMANAGERMEMORIA_H_

#include <map>
#include "BKDManager.h"


//implementacion para manejo de almacenamiento de nodos en memoria
class BKDManagerMemoria : public BKDManager
{
protected:
	std::map<int,BKDNodo*>	m_nodes;

public:
	BKDManagerMemoria(int capacidadNodoHoja, int capacidadNodoInterno);
	virtual ~BKDManagerMemoria();

	//!!Importante: Es responsabilidad del caller liberar la memoria devuelta
	virtual BKDNodo* GetNodoRaiz();
	virtual BKDNodo* GetNodoPrimeraHoja();
	virtual BKDNodo* GetNodo(int nroNodo);

	virtual BKDNodo* AgregarNodo(int nivel);
	virtual bool GuardarNodo(BKDNodo* nodo);
	virtual bool BorrarNodo(int nroNodo);

};


#endif /* BKDMANAGERMEMORIA_H_ */
