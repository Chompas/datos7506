/*
 * Constantes.h
 *
 *  Created on: 05/10/2010
 *      Author: nacho
 */

#ifndef CONSTANTES__H__
#define CONSTANTES__H__

/*
 * Archivo de constantes para el persistor
 */

const char BASURA = '/';									//Caracter que representa basura en un stream;
const int LONGITUD_MINIMA_REGISTRO_DE_LONGITUD_FIJA = 5; 	//Este numero se determina porque el registro va a tener
														 	//minimamente un dato de longitud=1 y un campo de control
															//que indica la longitud del dato que es un entero de
															//4 bytes.
const int CCRLF4 = 1;									 	//Campos de control de RegistroDeLongitudFija de 4 bytes
const int LCCRLF4 = CCRLF4 * 4; 							//Longitud de campos de control de RegistroDeLongitudFija
															//de 4 bytes

#endif /* CONSTANTES__H__ */
