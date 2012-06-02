/*
 * ArchivoBloques.h
 *
 *  Created on: Jun 1, 2012
 *      Author: vdiego
 */

#ifndef ARCHIVOBLOQUES_H_
#define ARCHIVOBLOQUES_H_

#include <string>
#include <fstream>
#include "Bloque.h"

class ArchivoBloques {
protected:
	std::string m_filePath;
	std::fstream m_file;
	int m_tamBloque;
	int m_cantBloques;
	int m_primerBloqueLibre;

	void LeerCamposControl();
	void GuardarCamposControl();
	int GetCampoControl(int offset);

	int GetPosBloque(int nroBloque);
	int GetNroBloquePrimerLibre();

public:
	static int GetTamanioMinimoBloque();

	ArchivoBloques(std::string filePath, int tamBloque);
	ArchivoBloques(std::string filePath);
	virtual ~ArchivoBloques();

	int GetTamanioTotalBloque();
	int GetTamanioDatosBloque();
	Bloque* GetBloque(int nroBloque); //nro Bloque de 1 en adelante. Si no se encuentra el nro de bloque, devuelve NULL

	Bloque* AgregarBloque(int& nroBloque); //Si no se puede agregar el bloque, devuelve NULL
	bool ActualizarBloque(int nroBloque, Bloque& bloque);
	bool EliminarBloque(int nroBloque);

};

#endif /* ARCHIVOBLOQUES_H_ */
