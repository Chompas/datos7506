/*
 * BKDClaves.h
 *
 *  Created on: May 31, 2012
 *      Author: vdiego
 */

#ifndef BKDCLAVES_H_
#define BKDCLAVES_H_

#include "../Disco/Serializable.h"
#include <vector>
#include <string>


class BKDClave : public Serializable
{
public :
	// -1: this < clave
	//  0: this == clave
	//  1: this > clave
	virtual int Comparar(const BKDClave& clave) const = 0;
	virtual BKDClave* Clonar() const = 0;
	virtual std::string ToString() const = 0;
};


class BKDClaveMultiple : public Serializable
{

public:
	int m_dimension;
	std::vector<BKDClave*> m_subclaves;

	// -1: this < clave
	//  0: this == clave
	//  1: this > clave
	virtual int Comparar(const BKDClaveMultiple& clave) const = 0;
	//virtual int CompararPorSubclave(const BKDClaveMultiple& clave, const int dimension) = 0;
	//virtual int CompararContraSubclave(const BKDClave& subclave, const int dimension) = 0;

	//!! Eliminar la memoria al terminar de usar la subclave !!
	virtual BKDClave* GetSubclave(int dimension) const = 0;
	virtual int GetDimension() const = 0;
	virtual std::string ToString() const = 0;


};



#endif /* BKDCLAVES_H_ */
