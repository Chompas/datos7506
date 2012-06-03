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

namespace std {

class ClaveAccidente : public ClaveInt {
public:
	ClaveAccidente(const int& accidente);

	virtual int Comparar(const BKDClave& clave) const;
};

} /* namespace std */
#endif /* CLAVEACCIDENTE_H_ */
