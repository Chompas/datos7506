/*
 * BKDManagerDisco.h
 *
 *  Created on: May 28, 2012
 *      Author: vdiego
 */

#ifndef BKDMANAGERDISCO_H_
#define BKDMANAGERDISCO_H_

#include "BKDManager.h"
#include "BKDInstanciador.h"
#include "../Disco/ArchivoBloques.h"

//implementacion para manejo de almacenamiento de nodos en disco
class BKDManagerDisco : public BKDManager
{
private:
	void RefrescarRaiz();
	BKDNodo* GetCopiaNodo(BKDNodo* nodo);
	void ActualizarCapacidadNodos(Bloque* bloque);

protected:
	ArchivoBloques* m_arch;
	BKDNodo* m_raiz;
	BKDInstanciador* m_instanciador;

public:
	BKDManagerDisco(const std::string& filePath, BKDInstanciador* instanciadorRegistros); //constructor solo de lectura de archivos preexistentes
	BKDManagerDisco(const std::string& filePath, const int tamanioBytesBloque, BKDInstanciador* instanciadorRegistros);
	virtual ~BKDManagerDisco();

	std::string GetFilePath();

	//!!Importante: Es responsabilidad del caller liberar la memoria devuelta
	virtual BKDNodo* GetNodoRaiz();
	virtual BKDNodo* GetNodoPrimeraHoja();
	virtual BKDNodo* GetNodo(int nroNodo);

	virtual BKDNodo* CrearNodoOffline(int nivel);
	virtual BKDRegistro* InstanciarRegistro();
	virtual int CalcularEspacioAOcupar(Registro* registro);

	virtual BKDNodo* AgregarNodo(int nivel);
	virtual bool GuardarNodo(BKDNodo* nodo);
	virtual bool BorrarNodo(int nroNodo);

};


#endif /* BKDMANAGERDISCO_H_ */
