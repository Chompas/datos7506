#ifndef __INTERNO_H__
#define __INTERNO_H__
#include "Hoja.h"

class Interno:public Nodo{
private:
	void**	claves;
	Nodo**	hijos;

public:
    Interno();
	
	int agregarClave(std::string elemento, Nodo* hd, Nodo * hi);
	int agregarElemento(std::string elemento);
	int obtenerNivel();
	std::string obtenerElemento(std::string elemento);
	bool estaVacio();
	
	virtual ~Interno();
	
};

#endif /* __INTERNO_H__ */
