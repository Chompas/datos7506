/*
 * ClaveInt.h
 *
 *  Created on: Jun 1, 2012
 *      Author: vdiego
 */

#ifndef CLAVEINT_H_
#define CLAVEINT_H_

#include "../Arbol/BKDClaves.h"
#include <string>

class ClaveInt : public BKDClave
{
public:
	int Valor;

	ClaveInt(int valor);
	ClaveInt(const ClaveInt& clave);
	virtual ~ClaveInt();


	//Implementacion de BKDClave
	// -1: this < clave
	//  0: this == clave
	//  1: this > clave
	virtual int Comparar(const BKDClave& clave) const;
	virtual BKDClave* Clonar() const;
	virtual std::string ToString() const;

	//Implementacion de Serializable
	virtual int getLongitud();
	virtual int serializar (Buffer* buffer, int posicion);
	virtual int hidratar (Buffer* buffer, int posicion);
};

#endif /* CLAVEINT_H_ */
