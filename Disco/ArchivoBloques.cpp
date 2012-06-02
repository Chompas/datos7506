/*
 * ArchivoBloques.cpp
 *
 *  Created on: Jun 1, 2012
 *      Author: vdiego
 */

#include "ArchivoBloques.h"

//************* METODOS ESTATICOS *************

int ArchivoBloques::GetTamanioMinimoBloque()
{
	return 20;
}

//*********************************************


int GetOffsetTamBloque()
{
	//TamBloque(int) + CantBloques(int) + PrimerBloqueLibre(int) + seccionBloques
	return 0;
}

int GetOffsetCantBloques()
{
	//TamBloque(int) + CantBloques(int) + PrimerBloqueLibre(int) + seccionBloques
	return sizeof(int);
}

int GetOffsetBloquesLibres()
{
	//TamBloque(int) + CantBloques(int) + PrimerBloqueLibre(int) + seccionBloques
	return sizeof(int) * 2;
}

int GetOffsetBloques()
{
	//TamBloque(int) + CantBloques(int) + PrimerBloqueLibre(int) + seccionBloques
	return sizeof(int) * 3;
}


//***************** METODOS PRIVADOS ************************

void ArchivoBloques::LeerCamposControl()
{
	//** Accede al archivo a los primeros bytes y obtiene la informacion de control
	//(tamanio bloque, cant bloques, bloqueslibres)


	//this->m_tamBloque = 0;
	//this->m_cantBloques = 0;
	//this->m_primerBloqueLibre = -1;
}

void ArchivoBloques::GuardarCamposControl()
{
	//Guarda los campos de control en el archivo
}

int ArchivoBloques::GetCampoControl(int offset)
{
	//file.seek(offset);
	//file.read(&campo, sizeof(int))
	//return campo
	return 0;
}

int ArchivoBloques::GetPosBloque(int nroBloque)
{
	return GetOffsetBloques() + (nroBloque * this->m_tamBloque);
}

int ArchivoBloques::GetNroBloquePrimerLibre()
{
	int posLibres = GetOffsetBloquesLibres();
	int nroBloque = 0;
	int pos = 0;

	//file.seek(posLibres);
	//file.read(&nroBloque, sizeof(int))
	return nroBloque;
}


//*************************************************








ArchivoBloques::ArchivoBloques(std::string filePath, int tamBloque)
{
	this->m_filePath = filePath;
	this->m_tamBloque = tamBloque;
	this->m_cantBloques = 0;
	this->m_primerBloqueLibre = -1;


	this->m_file.open(filePath.c_str(), fstream::out | fstream::binary | fstream::trunc | fstream::in);
	GuardarCamposControl();
}

ArchivoBloques::ArchivoBloques(std::string filePath)
{
	this->m_filePath = filePath;
	this->m_file.open(filePath.c_str(), fstream::out | fstream::binary | fstream::in);

	LeerCamposControl();
}

ArchivoBloques::~ArchivoBloques()
{
	if (this->m_file != NULL && this->m_file.is_open())
	{
		this->m_file.flush();
		this->m_file.close();
	}
}


int ArchivoBloques::GetTamanioTotalBloque()
{
	return this->m_tamBloque;
}

int ArchivoBloques::GetTamanioDatosBloque()
{
	//el tamanio del bloque menos los campos de control
	return this->m_tamBloque - sizeof(int);
}



//nro Bloque de 1 en adelante. Si no se encuentra el nro de bloque, devuelve NULL
Bloque* ArchivoBloques::GetBloque(int nroBloque)
{
	return NULL;

	//int pos = GetPosBloque(nroBloque);
	//file.seek(pos);
	//file.read(buffer, tamBloque);
	//bloque.hidratar(buffer)
	//nodo.cargar(bloque)
}

//Si no se puede agregar el bloque, devuelve NULL y -1 como numero de bloque
Bloque* ArchivoBloques::AgregarBloque(int& nroBloque)
{
	nroBloque = -1;
	return NULL;

	//Pedir nodo vacio
	//crear objeto nodo hoja/interno segun nivel, y asignarle el valor correspondiente de nro nodo
	//guardar los cambios hechos al archivo y devolver el nodo
}

bool ArchivoBloques::ActualizarBloque(int nroBloque, Bloque& bloque)
{
	return false;
}

bool ArchivoBloques::EliminarBloque(int nroBloque)
{
	return false;
}

