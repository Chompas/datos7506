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

	unsigned int Bloque::obtenerLimiteSuperior (){
		unsigned int limiteSuperior;

		//por la presencia del registro cero, el limite superior es variable
		if (!existeRegistroDeControl())
			limiteSuperior = this->registros.size();
		else
			limiteSuperior = (this->registros.size() - 1);

		return limiteSuperior;
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

		bool Bloque::actualizarRegistro (unsigned int posicionRegistro, Registro* registro){
			bool exito = false;

			if ((posicionRegistro > 0) && (posicionRegistro <= obtenerLimiteSuperior())){
				//se corre el indice de registros cuando no hay registro cero.
				if (!existeRegistroDeControl())
					posicionRegistro--;

				Registro* regAux = this->registros[posicionRegistro];
				int longitudAux = regAux->getLongitud();
				int nuevoEspacioLibre = obtenerEspacioLibre() + longitudAux;
				if ((nuevoEspacioLibre - registro->getLongitud()) >= 0){
					eliminarRegistro (posicionRegistro);
					insertarRegistro (registro);
					actualizarEspacioLibre (calcularEspacioLibre ());
					exito = true;
				}
			}

			return exito;
		}

		bool Bloque::actualizarRegistroDeControl (Registro* registroDeControl){
			bool exito = false;

			if (existeRegistroDeControl ()){
				Registro* regAux = this->registros[0];
				int longitudAux = regAux->getLongitud();
				int nuevoEspacioLibre = obtenerEspacioLibre() + longitudAux;
				if ((nuevoEspacioLibre - registroDeControl->getLongitud()) >= 0){
					delete (this->registros[0]);
					this->registros[0] = clonarRegistro(registroDeControl);
					actualizarEspacioLibre (calcularEspacioLibre ());
					exito = true;
				}
			}
			return exito;
		}

		bool Bloque::eliminarRegistro (unsigned int posicionRegistro){
			bool exito = false;

			if ((posicionRegistro > 0) && (posicionRegistro <= obtenerLimiteSuperior())){
				//se corre el indice de registros cuando no hay registro cero.
				if (!existeRegistroDeControl())
					posicionRegistro--;

				std::vector<Registro*>::iterator it = this->registros.begin();
				unsigned int contador = 0;
				while (( it != this->registros.end() ) && (contador < posicionRegistro)) {
					it++;
					contador++;
				}
				if (it != this->registros.end()){
					delete *it;
					this->registros.erase(it);
					actualizarEspacioLibre (calcularEspacioLibre ());
					exito = true;
				}
			}

			return exito;
		}

		Registro* Bloque::obtenerRegistro (unsigned int posicionRegistro){
			Registro* auxReg = NULL;

			if ((posicionRegistro > 0) && (posicionRegistro <= obtenerLimiteSuperior())){
				//se corre el indice de registros cuando no hay registro cero.
				if (!existeRegistroDeControl())
					posicionRegistro--;

				auxReg = this->registros[posicionRegistro];
				auxReg = clonarRegistro (auxReg);
			}

			return auxReg;
		}

		Registro* Bloque::obtenerRegistroDeControl (){
			Registro* regAux = NULL;

			if (existeRegistroDeControl())
				regAux = clonarRegistro (this->registros[0]);

			return regAux;
		}

		void Bloque::vaciar (){
			if (this->registros.size() > 0){
				std::vector<Registro*>::iterator it = this->registros.begin();

				for (unsigned int contador = 0; contador < this->registros.size(); contador++){
					delete (*it);
					it++;
				}
				this->registros.clear();

				actualizarEspacioLibre (calcularEspacioLibre ());
				desactivarRegistroDeControl();
			}
		}

		int Bloque::obtenerCantidadRegistros (){
			return (this->registros.size());
		}

		size_t Bloque::obtenerLongitud (){
			return this->longitud;
		}

		int Bloque::obtenerEspacioLibre (){
			return this->espacioLibre;
		}

		bool Bloque::existeRegistroDeControl (){
			return this->registroDeControl;
		}

		bool Bloque::vacio (){
			return (this->registros.size() == 0);
		}

		int Bloque::serializar (Buffer* buffer, int posicion){return 0;}

		int Bloque::hidratar (Buffer* buffer, int posicion){return 0;}

		bool Bloque::entra (Registro *registro){return 0;}
