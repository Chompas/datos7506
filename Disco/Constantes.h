/*
 * Constantes.h
 *
 *  Created on: 05/10/2010
 *      Author: nacho
 */
#ifndef CONSTANTES__H__
#define CONSTANTES__H__

#include <string>

using namespace std;

/*
 * Archivo de constantes para el persistor
 */

const int LBD = 512;										//Longitud de Bloque por defecto
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

const string RLFija = "RLF";								//Valor que identifica a un registro de longitud fija
const string RLVariable = "RLV";							//Valor que identifica a un registro de longitud variable

const string NOMBRE_ARCHIVO_CONTROL = "control.dat";			//nombre del archivo de control que utiliza la clase Archivo

#endif /* CONSTANTES__H__ */
