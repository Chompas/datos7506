#ifndef __HOJA_H__
#define __HOJA_H__
#include "Nodo.h"

class Hoja:public Nodo{
private:
	std::string*	registros;
	Hoja* 			vecino;
	
	//metodos internos
	int insertarElemento(std::string elemento);
	
public:
    Hoja(int capacidad);	

	int agregarElemento(std::string elemento);
	void distribuir(Nodo * unaHoja, std::string elemento);
	int obtenerNivel();
	std::string obtenerElemento(std::string elemento);
	bool estaVacio();
	
	virtual ~Hoja();
		
};

#endif /* __HOJA_H__ */
