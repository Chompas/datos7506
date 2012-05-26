#include <iostream>
#include <string>

#ifndef __NODO_H__
#define __NODO_H__

class Nodo {
protected:
	int		nivel;		  
	int		capacidad;
	int		dimension;
	int		cantidad;

public:
    Nodo(){};
	
	virtual int agregarElemento(std::string elemento) = 0;
	virtual int obtenerNivel() = 0;
	virtual std::string obtenerElemento(std::string elemento) = 0;	
	virtual bool estaVacio() = 0;
	
	virtual ~Nodo(){};
		
};

#endif /* __NODO_H__ */
