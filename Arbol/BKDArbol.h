/*
 * BKDArbol.h
 *
 *  Created on: May 27, 2012
 *      Author: vdiego
 */

#ifndef BKDARBOL_H_
#define BKDARBOL_H_

#include "BKDManager.h"
#include "BKDClaves.h"
#include "BKDRegistro.h"
#include "BKDInstanciador.h"

//representa un arbol B+ llevado a K-Dimensiones
class BKDArbol
{
protected:
	BKDManager* m_manager;

public:
	virtual ~BKDArbol();

	static BKDArbol* CrearEnMemoria(const int capacidadNodoHoja, const int capacidadNodoInterno); //en numero de registros y numero de claves, respectivamente
	static BKDArbol* CrearEnDisco(const std::string& filePath, const int tamanioBloqueBytes, BKDInstanciador* instanciadorRegistros); //Para rendimiento optimo utilizar bloques de 512 * 2^N Bytes
	static BKDArbol* AbrirDeDisco(const std::string& filePath, BKDInstanciador* instanciadorRegistros);

	bool BuscarRegistro(const BKDClave& clave, BKDRegistro** registro);
	bool BuscarPorRango(const BKDClave& claveInicio, const BKDClave& claveFin, std::list<BKDRegistro*>& resultado);

	bool InsertarRegistro(const BKDRegistro& registro);
	bool ModificarRegistro(const BKDRegistro& registro);
	bool EliminarRegistro(const BKDClave& clave);


	//PARA DEBUG
	virtual void DebugPrintAll();
	virtual void DebugPrint(int nivel);
};


#endif /* BKDARBOL_H_ */
