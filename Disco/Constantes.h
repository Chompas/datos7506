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
const int LCCBRLF = 10;										//Longitud en bytes del total de compos de control del bloque
															//con RLF
const int LCCBRLV = 6;										//Longitud en bytes del total de compos de control del bloque
															//con RLV
const int LCCRLV = 4;										//Longitud en bytes del campo de control de cada RLV.
const char S = 'S';											//Caracter que indica presencia de registro cero en stream.
const char N = 'N';											//Caracter que indica ausencia de registro cero en stream.
const int LR = 50;											//Tamanio de registro de longitud fija por defecto

#endif /* CONSTANTES__H__ */
