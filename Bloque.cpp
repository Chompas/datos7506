/*
 * Bloque.cpp
 *
 *  Created on: 23/05/2012
 *      Author: nacho
 */

#include "Bloque.h"

	void Bloque::actualizarEspacioLibre (size_t espacioLibre){}

	size_t Bloque::calcularEspacioLibre (){return 0;}

	Registro* Bloque::clonarRegistro (Registro* registro) {return 0;}

	/*
	*****************************************************************************************************
	* funciones publicas de la clase.
	*****************************************************************************************************
	*/

		Bloque::Bloque (size_t longitud){}

		Bloque::Bloque (Buffer* buffer, size_t longitud){}

		Bloque::~Bloque (){}

		bool Bloque::insertarRegistro (Registro *registro){return 0;}

		bool Bloque::actualizarRegistro (unsigned int posicionRegistro, Registro* registro){return 0;}

		bool Bloque::eliminarRegistro (unsigned int posicionRegistro){return 0;}

		Registro* Bloque::obtenerRegistro (unsigned int posicionRegistro){return 0;}

		void Bloque::vaciar (){}

		int Bloque::obtenerCantidadRegistros (){return 0;}

		size_t Bloque::obtenerLongitud (){return 0;}

		int Bloque::obtenerEspacioLibre (){return 0;}

		bool Bloque::vacio (){return 0;}

		int Bloque::serializar (Buffer* buffer, int posicion){return 0;}

		int Bloque::hidratar (Buffer* buffer, int posicion){return 0;}

		bool Bloque::entra (Registro *registro){return 0;}
