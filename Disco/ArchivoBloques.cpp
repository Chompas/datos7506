/*
 * ArchivoBloques.cpp
 *
 *  Created on: Jun 1, 2012
 *      Author: vdiego
 */

#include "ArchivoBloques.h"
#include "../Comun/Utils.h"

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
	Utils::LogDebug(Utils::dbgSS << "Leyendo Campos de control del archivo...");

	//Accede al archivo a los primeros bytes y obtiene la informacion de control
	//(tamanio bloque, cant bloques, bloqueslibres)

	this->m_tamBloque = this->GetCampoControl(GetOffsetTamBloque());
	this->m_cantBloques = this->GetCampoControl(GetOffsetCantBloques());
	this->m_primerBloqueLibre = this->GetCampoControl(GetOffsetBloquesLibres());

	Utils::LogDebug(Utils::dbgSS << "Leidos Campos de control del archivo..." << endl
								 << "\t\t Campo 'TamanioBloque', Valor '" << this->m_tamBloque << "', Posicion " << GetOffsetTamBloque() << endl
								 << "\t\t Campo 'CantidadBloques', Valor '" << this->m_cantBloques << "', Posicion " << GetOffsetCantBloques() << endl
								 << "\t\t Campo 'PrimerBloqueLibre', Valor '" << this->m_primerBloqueLibre << "', Posicion " << GetOffsetBloquesLibres());
}

void ArchivoBloques::GuardarCamposControl()
{
	Utils::LogDebug(Utils::dbgSS << "Guardando Campos de control del archivo..." << endl
								 << "\t\t Campo 'TamanioBloque', Valor '" << this->m_tamBloque << "', Posicion " << GetOffsetTamBloque() << endl
								 << "\t\t Campo 'CantidadBloques', Valor '" << this->m_cantBloques << "', Posicion " << GetOffsetCantBloques() << endl
								 << "\t\t Campo 'PrimerBloqueLibre', Valor '" << this->m_primerBloqueLibre << "', Posicion " << GetOffsetBloquesLibres());

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
	Utils::LogDebug(Utils::dbgSS << "Creando Archivo de Bloques...");

	this->m_filePath = filePath;
	this->m_tamBloque = tamBloque;
	this->m_cantBloques = 0;
	this->m_primerBloqueLibre = -1;


	this->m_file.open(filePath.c_str(), fstream::out | fstream::binary | fstream::trunc | fstream::in);

	if (this->m_file.fail())
	{
		Utils::LogError(Utils::errSS << "Error al intentar crear el archivo.");
		return; //? throw ex?
	}
	else
		Utils::LogDebug(Utils::dbgSS << "Archivo creado OK");

	GuardarCamposControl();
}

ArchivoBloques::ArchivoBloques(std::string filePath)
{
	Utils::LogDebug(Utils::dbgSS << "Leyendo Archivo de Bloques..." << endl
								 << "\t\t Archivo: " << filePath);

	this->m_filePath = filePath;
	this->m_file.open(filePath.c_str(), fstream::out | fstream::binary | fstream::in);

	if (this->m_file.fail())
	{
		Utils::LogError(Utils::errSS << "Error al intentar abrir el archivo.");
		return; //? throw ex?
	}

	LeerCamposControl();

	Utils::LogDebug(Utils::dbgSS << "Leido Archivo de Bloques, Tamanio Bloque: " << this->m_tamBloque);

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
	Utils::LogDebug(Utils::dbgSS << "Buscando en disco bloque nro: " << nroBloque);

	int pos = this->GetPosBloque(nroBloque);
	int tam = this->GetTamanioTotalBloque();
	this->m_file.seekg(pos, ios::beg);

	char* bf = new char[tam];
	this->m_file.read(bf, tam);

	if (this->m_file.gcount() != tam)
	{
		Utils::LogError(Utils::errSS << "Error al intentar leer el bloque " << nroBloque);
		return NULL;
	}

	Buffer buff = Buffer(bf, tam);
	delete[] bf;

	Bloque* bl = new Bloque(&buff, tam);

	if (bl == NULL)
		Utils::LogError(Utils::errSS << "Error al intentar obtener bloque " << nroBloque);

	return bl;
}

//Si no se puede agregar el bloque, devuelve NULL y -1 como numero de bloque
Bloque* ArchivoBloques::AgregarBloque(int& nroBloque)
{
	Utils::LogDebug(Utils::dbgSS << "Agregando bloque al archivo...");

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

	Utils::LogDebug(Utils::dbgSS << "Nro de bloque del nuevo bloque: " << nroBloque);

	Utils::LogDebug(Utils::dbgSS << "Intentando escribir bloque al archivo... (Tamanio: " << tamBF << ")");
	this->m_file.write(bf, tamBF);
	delete[] bf;

	if (this->m_file.fail())
	{
		Utils::LogError(Utils::errSS << "Error al intentar escribir bloque al archivo.");
		delete bloque;
		return NULL;
	}
	else
		Utils::LogDebug(Utils::dbgSS << "Bloque guardado OK.");

	this->m_cantBloques++;
	GuardarCamposControl();

	return bloque;
}

bool ArchivoBloques::ActualizarBloque(int nroBloque, Bloque& bloque)
{
	Utils::LogDebug(Utils::dbgSS << "Intentando guardar bloque a disco... (Nro de bloque: " << nroBloque << ")" );

	int pos = this->GetPosBloque(nroBloque);
	int tam = this->GetTamanioTotalBloque();

	Buffer buff = Buffer();

	Utils::LogDebug(Utils::dbgSS << "Serializando bloque...");
	if (bloque.serializar(&buff, 0) != 0)
	{
		Utils::LogError(Utils::errSS << "Error al serializar bloque.");
		return false;
	}
	Utils::LogDebug(Utils::dbgSS << "Bloque serializado OK.");

	int tamBF = 0;
	char* bf = buff.getStream(tamBF);

	if (tam != tamBF)
	{
		Utils::LogError(Utils::errSS << "Error al intentar actualizar bloque: el tamanio de bloque es incorrecto");
		if (bf != NULL)
			delete[] bf;
		return false;
	}

	Utils::LogDebug(Utils::dbgSS << "Escribiendo bloque al archivo... (offset: " << pos << ", tamanio: " << tamBF << ")" );

	this->m_file.seekp(pos, ios::beg);
	this->m_file.write(bf, tamBF);
	delete[] bf;

	if (this->m_file.fail())
	{
		Utils::LogError(Utils::errSS << "Error al intentar actualizar bloque: error al escribir al archivo");
		return false;
	}

	Utils::LogDebug(Utils::dbgSS << "Bloque escrito OK.");
	return true;
}

bool ArchivoBloques::EliminarBloque(int nroBloque)
{
	//this->m_cantBloques--;
	//GuardarCamposControl();

	return false;
}

