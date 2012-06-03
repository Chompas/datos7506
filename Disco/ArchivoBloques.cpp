/*
 * ArchivoBloques.cpp
 *
 *  Created on: Jun 1, 2012
 *      Author: vdiego
 */

#include "ArchivoBloques.h"
#include <iostream>

using namespace std;

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
	//Accede al archivo a los primeros bytes y obtiene la informacion de control
	//(tamanio bloque, cant bloques, bloqueslibres)

	this->m_tamBloque = this->GetCampoControl(GetOffsetTamBloque());
	this->m_cantBloques = this->GetCampoControl(GetOffsetCantBloques());
	this->m_primerBloqueLibre = this->GetCampoControl(GetOffsetBloquesLibres());
}

void ArchivoBloques::GuardarCamposControl()
{
	//Guarda los campos de control en el archivo
	this->SetCampoControl(GetOffsetTamBloque(), this->m_tamBloque);
	this->SetCampoControl(GetOffsetCantBloques(), this->m_cantBloques);
	this->SetCampoControl(GetOffsetBloquesLibres(), this->m_primerBloqueLibre);
}

int ArchivoBloques::GetCampoControl(int offset)
{
	this->m_file.seekg(offset, ios::beg);
	int res = 0;
	this->m_file.read((char*)&res, sizeof(int));
	return res;
}

void ArchivoBloques::SetCampoControl(int offset, int valor)
{
	this->m_file.seekg(offset, ios::beg);
	int res = valor;
	this->m_file.write((const char*)&res, sizeof(int));
}

int ArchivoBloques::GetPosBloque(int nroBloque)
{
	return GetOffsetBloques() + (nroBloque * this->m_tamBloque);
}

int ArchivoBloques::GetNroBloqueFromOffset(int offset)
{
	int result = offset;
	result -= GetOffsetBloques();
	result = result / this->m_tamBloque;
	return result;
}

int ArchivoBloques::GetNroBloquePrimerLibre()
{
	//int posLibres = GetOffsetBloquesLibres();
	int nroBloque = 0;
	//int pos = 0;

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
	//el tamanio del bloque menos los campos de control (puntero a siguiente bloque)
	return this->m_tamBloque - sizeof(int);
}

//nro Bloque de 1 en adelante. Si no se encuentra el nro de bloque, devuelve NULL
Bloque* ArchivoBloques::GetBloque(int nroBloque)
{
	int pos = this->GetPosBloque(nroBloque);
	int tam = this->GetTamanioTotalBloque();
	this->m_file.seekg(pos, ios::beg);

	char* bf = new char[tam];
	char* it = bf;
	this->m_file.read(it, tam);

	if (this->m_file.gcount() != tam)
	{
		cerr << "Error al intentar leer el bloque " << nroBloque << endl;
		return NULL;
	}

	Buffer buff = Buffer(bf, tam);
	Bloque* bl = new Bloque(&buff, tam);

	if (bl == NULL)
		cerr << "Error al intentar obtener bloque " << nroBloque << endl;

	return bl;
}

//Si no se puede agregar el bloque, devuelve NULL y -1 como numero de bloque
Bloque* ArchivoBloques::AgregarBloque(int& nroBloque)
{
	nroBloque = -1;
	Bloque* bloque = NULL;
	int pos = 0;

	//**TODO: Reusar nodos libres

	bloque = new Bloque(this->m_tamBloque);
	Buffer buff = Buffer();
	bloque->serializar(&buff, 0);
	int tamBF = 0;
	char* bf = buff.getStream(tamBF);

	this->m_file.seekp(0, ios::end);
	pos = this->m_file.tellp();
	nroBloque = this->GetNroBloqueFromOffset(pos);

	this->m_file.write(bf, tamBF);
	delete[] bf;

	this->m_cantBloques++;
	GuardarCamposControl();

	return bloque;
}

bool ArchivoBloques::ActualizarBloque(int nroBloque, Bloque& bloque)
{
	int pos = this->GetPosBloque(nroBloque);
	int tam = this->GetTamanioTotalBloque();

	Buffer buff = Buffer();
	bloque.serializar(&buff, 0);
	int tamBF = 0;
	char* bf = buff.getStream(tamBF);

	if (tam != tamBF)
	{
		cerr << "Error al intentar actualizar bloque: el tamanio de bloque es incorrecto" << endl;
		if (bf != NULL)
			delete[] bf;
		return false;
	}

	this->m_file.seekp(pos, ios::beg);
	this->m_file.write(bf, tamBF);
	delete[] bf;

	return true;
}

bool ArchivoBloques::EliminarBloque(int nroBloque)
{
	//this->m_cantBloques--;
	//GuardarCamposControl();

	return false;
}

