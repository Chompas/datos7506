/*
 * InstanciadorIncidentes.h
 *
 *  Created on: Jun 2, 2012
 *      Author: vdiego
 */

#ifndef INSTANCIADORINCIDENTES_H_
#define INSTANCIADORINCIDENTES_H_

#include "../Arbol/BKDInstanciador.h"

class InstanciadorIncidentes : public BKDInstanciador
{
public:
	InstanciadorIncidentes();
	virtual ~InstanciadorIncidentes();

	virtual BKDRegistro* InstanciarRegistro() const;
	virtual BKDClaveMultiple* InstanciarClave() const;
};

#endif /* INSTANCIADORINCIDENTES_H_ */
