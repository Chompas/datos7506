/*
 * ClaveAccidente.h
 *
 *  Created on: Jun 3, 2012
 *      Author: chompas
 */

#ifndef CLAVEACCIDENTE_H_
#define CLAVEACCIDENTE_H_

#include "ClaveInt.h"
#include <string>


class ClaveAccidente : public ClaveInt {
public:
	ClaveAccidente(const int& accidente);

	virtual int Comparar(const BKDClave& clave) const;
	virtual std::string ToString() const;
};

#endif /* CLAVEACCIDENTE_H_ */
