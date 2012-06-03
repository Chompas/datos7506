/*
 * Bloque.h
 *
 *  Created on: 23/05/2012
 *      Author: nacho
 */

#ifndef BLOQUE_H_
#define BLOQUE_H_
#include "Serializable.h"
#include "Buffer.h"
#include "Registro.h"
#include <vector>
#include <stdlib.h>

/*
 * Bloque: Modela un bloque de un Archivo, con una longitud en bytes fijo y multiplo de 512 (512 * 2 ^ x).
 * Un bloque contiene Registros. El formato es [int: cantidad de Registros][int: espacio libre]
 * [char: 'S' Registros de Longitud Fija | 'N' No Registros de Longitud Fija][int: tamanio Registro Longitud Fija]/[int: tamanio Registro 1]
 * [Registro: registro 1][int: tamanio registro 2][Registro: registro 2]...
 */
class Bloque : public Serializable{
private:
	std::vector<Registro*> registros;		//vector donde se almacenan los registros del bloque.
	int longitud;							//variable donde se almacena la longitud del bloque en el archivo.
											//Debe ser multiplo de 512.
	int espacioLibre;						//variable donde se almacena la cantidad de bytes libres al final
											//del bloque.
	bool RLF;								//variable que indica si los registros en el bloque son de long. fija (True)
											//o no (False).
	int longitudRLF;						//variable que indica la longitud de los registros de longitud fija. Si
											//son de long. variable, el valor es 0 y no se lo incluye en el stream.
	bool registroDeControl;					//Variable que indica se el bloque tiene activo el registro de control
											//(true), o no (false). registroDeControl es un registro especial donde
											//el usuario puede guardar en el bloque informacion adicional
											//distinta a la del resto de los registros. Como es especial, para
											//su uso habra primitivas especiales. El registro de control no se puede
											//eliminar individualmente, solo vaciando el bloque.


	/*
	 * Activa la señal de existencia de registro 0
	 * pre: Debe haber sido insertado el registro 0.
	 * pos: Se activa la señal de existencia de registro 0;
	 */
	void activarRegistroDeControl ();

	/*
	 * Desactiva la señal de existencia de registro 0
	 * pre: Debe haber sido eliminado el registro 0.
	 * pos: Se desactiva la señal de existencia de registro 0;
	 */
	void desactivarRegistroDeControl ();

	/*
	* Establece la cantidad de espacio libre en bytes que hay al final del bloque.
	* pre: espacioLibre debe ser >= 0 y <= que el tamanio del bloque - 2. Los 2 primeros, se usan como
	* 	   campos de control.
	* pos: Almacena la cantidad de espacio libre en bytes que hay al final del bloque.
	*
	* espacioLibre: Variable entera >= 0 que representa el espacio libre en bytes al final del bloque.
	*/
	void actualizarEspacioLibre (size_t espacioLibre);

	/*
	 * Calcula el espacio libre en el bloque.
	 * pre: -
	 * pos: Devuelve el calculo del espacio libre que hay al final del bloque.
	 *
	 */
	size_t calcularEspacioLibre ();

	/*
	 * Obtiene el limite superior de la cantidad de registros, dado que este es variable segun exista o no
	 * el registro de control.
	 * pre: -
	 * pos: Devuelve el limite superior de la cantidad de registros.
	 */
	unsigned int obtenerLimiteSuperior ();

	/*
	 * Clona un registro.
	 * pre: registro debe ser una referancia a un registro distinto de NULL.
	 * pos: Revuelve una copia de registro. EL USUARIO DEBE ENCARGARSE DE LIBERAR LA MEMORIA UTILIZADA.
	 */
	Registro* clonarRegistro (Registro* registro);

	/*
	 * setea el tipo de registros que va a recibir el bloque
	 * pre: registro debe ser una referancia a un registro distinto de NULL.
	 * pos: Establece si los registros en el bloque seran de long fija o variable
	 */
	void setRLF (Registro *registro);

	/*
	*****************************************************************************************************
	* funciones publicas de la clase.
	*****************************************************************************************************
	*/

	public:
		/*
		 * Constructor de Bloque con un parametros.
		 * pre: longitud debe ser un entero multiplo de 512.
		 * pos: El bloque queda instanciado para su uso. Su longitud sera igual a longitud.
		 *
		 * longitud: La longitud del bloque en el archivo. Debe ser multiplo de 512.
		 */
		Bloque (size_t longitud);

		/*
		 * Constructor de Bloque con dos parametros. Se usa para pasarle un buffer de bloque.
		 * pre: Debe tener el formato [int: cantidad de Registros][int: espacio libre]
		 * [char: 'S' existe registro de control | 'N' no existe registro de control][int: tamanio Registro 1]
		 * [Registro: registro 1][int: tamanio registro 2][Registro: registro 2]...
		 * pos: El bloque queda instanciado para su uso. Contiene la informacion pasada en el stream.
		 *
		 * stream: puntero a un stream para cargar en bloque. Debe tener el formato [int: cantidad de Registros]
		 * [int: espacio libre][char: 'S' existe registro de control | 'N' no existe registro de control][int: tamanio Registro 1]
		 * [Registro: registro 1][int: tamanio registro 2][Registro: registro 2]...
		 */
		Bloque (Buffer* buffer, size_t longitud);

		/*
		 * Destructor de Bloque.
		 * pre: -
		 * pos: Se liberan los recursos usados por la instancia. Se eliminan todos los registros de la memoria.
		 */
		virtual ~Bloque ();

		/*
		 * Inserta registro en el bloque.
		 * pre: registro debe ser distinto de NULL.
		 * pos: Si hay espacio suficiente en el bloque, se agrega una copia del registro al final del bloque y
		 * 		se devuelve true. De lo contrario, no se inserta y devuelve false.
		 *
		 * registro: puntero a Registro.
		 */
		bool insertarRegistro (Registro *registro);

		/*
		 * Inserta el registro de control en el bloque.
		 * pre: registro debe ser distinto de NULL. No debe existir el registro de control en el bloque. Si existe,
		 * 		no hace nada.
		 * pos: Si hay espacio suficiente en el bloque, se agrega una copia del registro y se devuelve true.
		 * 		De lo contrario, no se inserta y devuelve false.
		 *
		 * registroDeControl: puntero a Registro.
		 */
		bool insertarRegistroDeControl (Registro *registroDeControl);

		/*
		 * Actualiza el registro de la posicion posicionRegistro con registro;
		 * pre: posicionRegistro debe ser una posicion de registro valido y > 0. registro debe ser una referencia a un registro.
		 * 		Debe der distinto de NULL.
		 * pos: Devuelve true si se pudo realizar la actualizacion con exito (se elimina el registro antiguo y se inserta al
		 * 		final el nuevo). De lo contrario, no se hace nada y devuelve false.
		 *
		 * posicionRegistro: Entero que representa la posicion de un registro. debe ser > 0.
		 * registro: puntero a un registro.
		 */
		bool actualizarRegistro (unsigned int posicionRegistro, Registro* registro);

		/*
		 * Actualiza el registro de control con registroDeControl;
		 * pre: Debe existir el registro cero. Si no existe, no hace nada. registro debe ser una referencia a un registro.
		 * 		Debe der distinto de NULL.
		 * pos: Devuelve true si se pudo realizar la actualizacion con exito. De lo contrario, no se hace nada y devuelve false.
		 *
		 * registroDeControl: puntero a un registro.
		 */
		bool actualizarRegistroDeControl (Registro* registroDeControl);

		/*
		 * Elimina el registro en la posicion posicionRegistro
		 * pre: posicionRegistro debe ser una posicion de registro valido. Debe ser > 0.
		 * pos: Si posicionRegistro es una posicion valida, elimina el registro en la posicion posicionRegistro y devuelve true.
		 * En caso contrario no hace nada y devuelve false.
		 *
		 * posicionRegistro: posicion del registro dentro del bloque. Debe ser > 0.
		 */
		bool eliminarRegistro (unsigned int posicionRegistro);

		/*
		 * Devuelve una copia del registro en la posicion posicionRegistro.
		 * pre: posicionRegistro debe ser una posicion de registro valido.
		 * pos: Si posicionRegistro es una posicion valida, devuelve la referencia al registro en la posicion posicionRegistro.
		 * 		En caso contrario devuelve NULL. EL USUARIO DEBE ENCARGARSE DE LIBERAR LA MEMORIA UTILIZADA.
		 *
		 * posicionRegistro: posicion del registro dentro del bloque.
		 */
		Registro* obtenerRegistro (unsigned int posicionRegistro);

		/*
		 * Devuelve una copia del registro de control.
		 * pre: Debe existir el registro de control.
		 * pos: Se el registro cero existe, devuelve la referencia al registro cero. En caso contrario devuelve NULL.
		 * 		EL USUARIO DEBE ENCARGARSE DE LIBERAR LA MEMORIA UTILIZADA.
		 */
		Registro* obtenerRegistroDeControl ();

		/*
		 * Elimina todos los registros del bloque, incluyendo el registro de control.
		 * pre: Debe existir al menos un registro en el bloque.
		 * pos: Elimina todos los registros del bloque. Libera la memoria utilizada por los registros.
		 */
		void vaciar ();

		/*
		 * Devuelve la cantidad de registros en el bloque.
		 * pre: -
		 * pos: Devuelve la cantidad de registros en el bloque.
		 */
		int obtenerCantidadRegistros ();

		/*
		 * Devuelve la longitud del bloque.
		 * pre: -
		 * pos: Devuelve la longitud del bloque.
		 */
		int getLongitud ();

		/*
		 * Obtiene la posicion en el stream del bloque del primer byte libre del bloque.
		 * pre: -
		 * pos: Se devuelve la posicion en el stream del bloque del primer byte libre.
		 */
		int obtenerEspacioLibre ();

		/*
		 * Indica se existe el registro de control en el bloque.
		 * pre: -
		 * pos: Devuelve true si existe el registro de control en el bloque. De lo contrario, devuelve false.
		 */
		bool existeRegistroDeControl ();

		/*
		 * Indica si el bloque esta vacio.
		 * pre: -
		 * pos: Si el bloque no contiene registros, devuelve true. De lo contrario, devuelve false.
		 */
		bool vacio ();

		/*
		 * Transforma el bloque en un srteam de bytes.
		 * pre: -
		 * pos: Devuelve el bloque convertido en un stream. EL USUARIO DEBE ENCARGARSE DE LIBERAR LA MEMORIA UTILIZADA.
		 */
		int serializar (Buffer* buffer, int posicion);

		/*
		 * Transforma un stream de bytes en un Bloque. Lo hace sobre la propia instancia desde la que se invoca el metodo.
		 * pre: La longitud del stream debe ser igual a la longitud del bloque. Debe tener el formato logico
		 * [int: cantidad de Registros][int: espacio libre][char: 'S' existe registro de control | 'N' no existe registro de control]
		 * [int: tamanio Registro 1][Registro: registro 1][int: tamanio registro 2][Registro: registro 2]...
		 * pos: devuelve la referencia al objeto Bloque propiamente dicho.
		 *
		 * stream: Puntero a un stream para cargar en bloque. Debe tener el formato logico [int: cantidad de Registros]
		 * [int: espacio libre][char: 'S' existe registro de control | 'N' no existe registro de control]
		 * [int: tamanio Registro 1][Registro: registro 1][int: tamanio registro 2][Registro: registro 2]...
		 */
		int hidratar (Buffer* buffer, int posicion);

		/*
		 * Indica si un registro entra en el espacio libre del bloque.
		 * pre: registro debe ser una referencia a un Registro distinta de NULL.
		 * pos: Si el registro entra en el bloque, devuelve true. De lo contrario, devuelve false.
		 *
		 * registro: puntero a Registro.
		 */
		bool entra (Registro *registro);


		bool EsRLF ();

};

#endif /* BLOQUE_H_ */
