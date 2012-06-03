/*
 * ClaveLinea.h
 *
 *  Created on: Jun 3, 2012
 *      Author: chompas
 */

#ifndef CLAVELINEA_H_
#define CLAVELINEA_H_

#include "ClaveInt.h"
#include <string>

namespace std {

class ClaveLinea : public ClaveInt {
public:
	virtual int Comparar(const BKDClave& clave) const;
};

} /* namespace std */
#endif /* CLAVELINEA_H_ */
