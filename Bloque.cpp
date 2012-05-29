/*
 * Bloque.cpp
 *
 *  Created on: 23/05/2012
 *      Author: nacho
 */

#include "Bloque.h"
#include "Constantes.h"

	void Bloque::activarRegistroDeControl (){
		this->registroDeControl = true;
	}

	void Bloque::desactivarRegistroDeControl (){
		this->registroDeControl = false;
	}

	void Bloque::actualizarEspacioLibre (size_t espacioLibre){
		this->espacioLibre = espacioLibre;
	}

	size_t Bloque::calcularEspacioLibre (){
		size_t espacioOcupado;

		if (this->RLF)
			espacioOcupado = LCCBRLF;
		else
			espacioOcupado = LCCBRLF;

		if (this->registros.size() > 0){
			std::vector<Registro*>::iterator it = this->registros.begin();

			for (unsigned int contador = 0; contador < this->registros.size(); contador++){
				if (this->RLF)
					espacioOcupado += (*it)->getLongitud();
				else
					espacioOcupado += (*it)->getLongitud() + LCCRLV;
				it++;
			}
		}

		return (obtenerLongitud() - espacioOcupado);
	}

	Registro* Bloque::clonarRegistro (Registro* registro) {return 0;}

	/*
	*****************************************************************************************************
	* funciones publicas de la clase.
	*****************************************************************************************************
	*/

		Bloque::Bloque (size_t longitud){
			this->longitud = longitud;
			this->RLF = false;
			this->longitudRLF = 0;
			this->registroDeControl = false;
			this->espacioLibre = longitud - LCCBRLV; //se le restan a la longitud del bloque la long. de
													 //los campos de control.
		}

		Bloque::Bloque (Buffer* buffer, size_t longitud){
			this->longitud = longitud;
			hidratar(buffer, 0);
		}

		Bloque::~Bloque (){
			vaciar();
		}

		bool Bloque::insertarRegistro (Registro *registro){
			bool exito = false;

			if (entra (registro)){
				this->registros.push_back(clonarRegistro(registro));
				actualizarEspacioLibre (calcularEspacioLibre ());
				exito = true;
			}

			return exito;
		}

		bool Bloque::insertarRegistroDeControl (Registro *registroDeControl){
			bool exito = false;

			if (!existeRegistroDeControl()){
				if (this->registros.size() == 0)
					exito = insertarRegistro (registroDeControl);
				else if ((this->registros.size() > 0) && entra(registroDeControl)){
					std::vector<Registro*>::iterator it = this->registros.begin();
					this->registros.insert(it, clonarRegistro(registroDeControl));
					actualizarEspacioLibre (calcularEspacioLibre ());
					exito = true;
				}
			}

			if (exito)
				activarRegistroDeControl();

			return exito;
		}

		bool Bloque::actualizarRegistro (unsigned int posicionRegistro, Registro* registro){return 0;}

		bool Bloque::actualizarRegistroDeControl (Registro* registroDeControl){return 0;}

		bool Bloque::eliminarRegistro (unsigned int posicionRegistro){return 0;}

		Registro* Bloque::obtenerRegistro (unsigned int posicionRegistro){return 0;}

		Registro* Bloque::obtenerRegistroDeControl (){return 0;}

		void Bloque::vaciar (){}

		int Bloque::obtenerCantidadRegistros (){return 0;}

		size_t Bloque::obtenerLongitud (){return 0;}

		int Bloque::obtenerEspacioLibre (){return 0;}

		bool Bloque::existeRegistroDeControl (){return 0;}

		bool Bloque::vacio (){return 0;}

		int Bloque::serializar (Buffer* buffer, int posicion){return 0;}

		int Bloque::hidratar (Buffer* buffer, int posicion){return 0;}

		bool Bloque::entra (Registro *registro){return 0;}
