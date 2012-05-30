/*
 * Bloque.cpp
 *
 *  Created on: 23/05/2012
 *      Author: nacho
 */

#include <string.h>
#include "Bloque.h"
#include "RegistroDeLongitudFija.h"
#include "RegistroDeLongitudVariable.h"
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

	Registro* Bloque::clonarRegistro (Registro* registro) {
		Buffer* regBuf = new Buffer;
		Registro* auxReg;

		registro->serializar(regBuf, 0);
		if (this->RLF){
			auxReg = new RegistroDeLongitudFija(registro->getLongitud());
			auxReg->hidratar(regBuf,0);
			}
		else{
			auxReg = new RegistroDeLongitudVariable();
			auxReg->hidratar(regBuf, 0);
			}

		delete regBuf;

		return auxReg;
	}

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

		int Bloque::serializar (Buffer* buffer, int posicion){
			char* stream;
			char* ptr;
			int cantidadRegistros = obtenerCantidadRegistros();
//			int espaciolibre = obtenerEspacioLibre();
			stream = new char[obtenerLongitud()];

			//se carga todo el buffer con basura
			memset (stream, BASURA, this->obtenerLongitud());

			//se cargan los campos de control
			ptr = stream;
			//cantidad de registros
			memcpy(ptr, &cantidadRegistros, sizeof (int));
			ptr += sizeof (int);

/*			//espacio libre
			memcpy(ptr, &espaciolibre, sizeof (int));
			ptr += sizeof (int);
*/
			//registro de lomgitud fija
			switch (this->RLF){
			case true:  memcpy (ptr, &S, sizeof(char));
						break;
			case false: memcpy (ptr, &N, sizeof(char));
						break;
			}
			ptr++;

			//Si los registros son de LF se agrega la longitud
			if (this->RLF){
				memcpy (ptr, &(this->longitudRLF), sizeof(int));
				ptr++;
			}

			//presencia de registro de control
			switch (existeRegistroDeControl()){
			case true:  memcpy (ptr, &S, sizeof(char));
						break;
			case false: memcpy (ptr, &N, sizeof(char));
						break;
			}
			ptr++;

			//registros
			if (!registros.empty()){
				for (unsigned int indiceReg = 0; indiceReg < registros.size(); indiceReg++){
					Buffer* regBuf = new Buffer;
					registros[indiceReg]->serializar(regBuf, 0);
					int longitud;
					char* streamRegistro = regBuf->getStream(longitud);
					int longitudRegistro = registros[indiceReg]->getLongitud();

					memcpy(ptr, &longitudRegistro, sizeof (int));
					ptr += sizeof(int);
					memcpy (ptr, streamRegistro, longitudRegistro);
					ptr+= longitudRegistro;

					delete regBuf;
					delete []streamRegistro;
					}
			}
			buffer->setStream(stream, obtenerLongitud());
			delete []stream;
			return 0;

		}

		int Bloque::hidratar (Buffer* buffer, int posicion){
			int longitud;
			char* stream = buffer->getStream(longitud);

			if (stream != NULL){
				//carga campos de control
				char* ptr = stream;
				int cantidadRegistros;
//				int espacioLibre;
				char RLF;
				int longitudRLF;
				char registroDeControl;

				//se vacia el bloque para dejarlo limpio.
				vaciar();

				memcpy(&cantidadRegistros, ptr, sizeof(int));
				ptr += sizeof(int);

/*				memcpy(&espacioLibre, ptr, sizeof(int));
				actualizarEspacioLibre (espacioLibre);
				ptr += sizeof(int);
*/
				memcpy(&RLF, ptr, sizeof(char));
				switch (RLF){
				case S: this->RLF = true;
						break;
				case N: this->RLF = false;
						break;
				}
				ptr += sizeof(char);

				if (this->RLF){
					memcpy(&longitudRLF, ptr, sizeof(int));
					ptr += sizeof(int);
				}

				memcpy(&registroDeControl, ptr, sizeof(char));
				switch (registroDeControl){
				case S: activarRegistroDeControl();
						break;
				case N: desactivarRegistroDeControl();
						break;
				}
				ptr += sizeof(char);

				//carga los registros
				for (int indice = 0; indice < cantidadRegistros; indice++){
					int longitudRegistro;
					memcpy(&longitudRegistro, ptr, sizeof(int));
					ptr += sizeof(int);

					char* streamRegistro = new char[longitudRegistro];

					memcpy (streamRegistro, ptr, longitudRegistro);
					Buffer* regBuf = new Buffer;
					regBuf->setStream(streamRegistro, longitudRegistro);
						Registro* auxReg;

						if (this->RLF){
							auxReg = new RegistroDeLongitudFija(longitudRegistro);
							auxReg->hidratar(regBuf,0);
							}
						else{
							auxReg = new RegistroDeLongitudVariable();
							auxReg->hidratar(regBuf, 0);
							}

					insertarRegistro (auxReg);
					ptr += longitudRegistro;

					delete regBuf;
					delete auxReg;
					delete[] streamRegistro;
				}
			}

			actualizarEspacioLibre (this->calcularEspacioLibre());
			delete []stream;
			return 0;
		}

		bool Bloque::entra (Registro *registro){
			int longitudStreamRegistro = registro->getLongitud();

			bool entraTemp = false;

			if ((obtenerEspacioLibre() - longitudStreamRegistro) >= 0)
				entraTemp = true;

			return entraTemp;
		}
