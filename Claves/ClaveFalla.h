/*
 * ClaveFalla.h
 *
 *  Created on: Jun 3, 2012
 *      Author: chompas
 */

#ifndef CLAVEFALLA_H_
#define CLAVEFALLA_H_

#include "ClaveInt.h"
#include <string>

namespace std {

class ClaveFalla : public ClaveInt {
public:
	ClaveFalla(const int& falla);

	virtual int Comparar(const BKDClave& clave) const;
};

} /* namespace std */
#endif /* CLAVEFALLA_H_ */
