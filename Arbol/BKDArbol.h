/*
 * BKDArbol.h
 *
 *  Created on: May 27, 2012
 *      Author: vdiego
 */

#ifndef BKDARBOL_H_
#define BKDARBOL_H_

#include "BKDManager.h"

//representa un arbol B+ llevado a K-Dimensiones
class BKDArbol
{
protected:
	BKDManager* m_manager;

public:
	virtual ~BKDArbol();

	static BKDArbol* CrearEnMemoria(const int capacidadNodoHoja, const int capacidadNodoInterno); //en numero de registros y numero de claves, respectivamente
	static BKDArbol* CrearEnDisco(const std::string& filePath, const int tamanioBloqueBytes); //Para rendimiento optimo utilizar bloques de 512 * 2^N Bytes
	static BKDArbol* AbrirDeDisco(const std::string& filePath);

	bool BuscarRegistro(const TClave& clave, TRegistro& registro);
	bool BuscarPorRango(const TClave& claveInicio, const TClave& claveFin, std::list<TRegistro>& resultado);

	bool InsertarRegistro(const TRegistro& registro);
	bool ModificarRegistro(const TRegistro& registro);
	bool EliminarRegistro(const TClave& clave);


	//PARA DEBUG
	virtual void DebugPrintAll();
	virtual void DebugPrint(int nivel);
};


#endif /* BKDARBOL_H_ */
