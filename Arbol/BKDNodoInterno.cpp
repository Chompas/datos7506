/*
 * BKDNodoInterno.cpp
 *
 *  Created on: May 27, 2012
 *      Author: vdiego
 */

#include "BKDNodoInterno.h"

#include <iostream>

using namespace std;

typedef list<BKDClaveMultiple*>::iterator ClavesIterator;
typedef list<int>::iterator HijosIterator;


BKDNodoInterno::~BKDNodoInterno()
{
	if (this->m_claves.size() > 0)
	{
		//recorrer los registros y deletearlos
		for (ClavesIterator it = this->m_claves.begin(); it != this->m_claves.end(); it++)
		{
			delete *it;
		}

		this->m_claves.clear();
	}
}

BKDNodoInterno::BKDNodoInterno(BKDManager* manager, int nroNodo, int capacidad, int nivel)
{
	this->m_capacidad = capacidad;
	this->m_nivel = nivel;
	this->m_manager = manager;
	this->m_nro_nodo = nroNodo;
	this->m_claves.clear();
	this->m_hijos.clear();
}

BKDNodoInterno::BKDNodoInterno(const BKDNodoInterno& ref)
{
	this->m_capacidad = ref.m_capacidad;
	this->m_nivel = ref.m_nivel;
	this->m_manager = ref.m_manager;
	this->m_nro_nodo = ref.m_nro_nodo;
	this->m_claves.clear();

	for (std::list<BKDClaveMultiple*>::const_iterator i = ref.m_claves.begin(); i != ref.m_claves.end(); i++)
	{
		this->m_claves.push_back((*i)->Clonar());
	}

	this->m_hijos.clear();
	this->m_hijos.assign(ref.m_hijos.begin(), ref.m_hijos.end());
}

bool BKDNodoInterno::HayOverflow()
{
	return (this->m_claves.size() > this->m_capacidad);
}

bool BKDNodoInterno::HayUnderflow()
{
	//si soy raiz, no tengo un minimo de registros, siempre puedo eliminar.
	if (this->m_nivel == 0)
		return false;

	//sino, tengo que asegurar tener al menos capacidad / 2 claves.
	return (this->m_claves.size() < (this->m_capacidad / 2));
}

int BKDNodoInterno::CantidadAMover()
{
	return ((this->m_capacidad / 2) + 1); //+ 1 xq copio el hijo y la clave a promover
}

bool BKDNodoInterno::BuscarReg(const BKDClaveMultiple& clave, BKDRegistro** registro, int profundidad)
{
	int dimension = profundidad % clave.GetDimensiones();
	BKDClave* subclave = clave.GetSubclave(dimension);

	//Recorro en orden las claves hasta encontrar la primera mayor o igual.
	//Si llegue al final, continuo por el ultimo hijo.

	HijosIterator hi = this->m_hijos.begin();
	ClavesIterator ci = this->m_claves.begin();

	while (ci != this->m_claves.end() && (*ci)->CompararPorSubclave(*subclave, dimension) != 1)
	{
		ci++;
		hi++;
	}

	delete subclave;

	//si por algun motivo llegue al final de la lista de hijos, hay algo mal en el árbol
	if (hi == this->m_hijos.end())
	{
		cerr << "Error: Cantidad de hijos invalida en nodo " << this->m_nro_nodo << endl;
		return false;
	}

	BKDNodo* hijo = this->m_manager->GetNodo(*hi);

	if (hijo == NULL)
	{
		cerr << "Error: En nodo "<< this->m_nro_nodo << " - Puntero a nodo hijo inexistente: " << *hi << endl;
		return false;
	}

	bool res = hijo->BuscarReg(clave, registro, profundidad + 1);
	delete hijo;
	return res;
}

bool BKDNodoInterno::BuscarRango(const BKDClaveMultiple& claveInicio, const BKDClaveMultiple& claveFin, std::list<BKDRegistro*>& resultado, int profundidad)
{
	int dimension = profundidad % claveInicio.GetDimensiones();

	BKDClave* subclaveInicio = claveInicio.GetSubclave(dimension);
	BKDClave* subclaveFin = claveFin.GetSubclave(dimension);

	//recorro mis claves, desde la primera mayor o igual a mi desde, y corto cuando encuentro la primera mayor a mi hasta.
	//en cada hijo de la clave en la que estoy parado, llamo de vuelta al metodo buscar Rango.

	ClavesIterator ci = this->m_claves.begin();
	HijosIterator hi = this->m_hijos.begin();
	bool enIzquierdos = true;

	while (ci != this->m_claves.end() && (*ci)->CompararPorSubclave(*subclaveInicio, dimension) == -1)
	{
		ci++;
		hi++;
	}

	if ((*ci)->CompararPorSubclave(*subclaveInicio, dimension) == 0)
	{
		hi++;
		enIzquierdos = false;
	}

	delete subclaveInicio;
	subclaveInicio = NULL;

	while (ci != this->m_claves.end() && (*ci)->CompararPorSubclave(*subclaveFin, dimension) != 1)
	{
		BKDNodo* hijo = this->m_manager->GetNodo(*hi);

		if (hijo == NULL)
		{
			cerr << "Error: En nodo "<< this->m_nro_nodo << " - Puntero a nodo hijo inexistente: " << *hi << endl;
			delete subclaveFin;
			return false;
		}

		hijo->BuscarRango(claveInicio, claveFin, resultado, profundidad + 1);
		delete hijo;

		ci++;
		hi++;
	}

	delete subclaveFin;
	subclaveFin = NULL;

	//Si me falto el ultimo hijo derecho...
	if (enIzquierdos && hi != this->m_hijos.end())
	{
		BKDNodo* hijo = this->m_manager->GetNodo(*hi);

		if (hijo == NULL)
		{
			cerr << "Error: En nodo "<< this->m_nro_nodo << " - Puntero a nodo hijo inexistente: " << *hi << endl;
			return false;
		}

		hijo->BuscarRango(claveInicio, claveFin, resultado, profundidad + 1);
		delete hijo;
	}

	return true;
}


bool BKDNodoInterno::InsertarReg(const BKDRegistro& registro, bool& overflow, int profundidad)
{
	overflow = false;

	//soy nodo interno, primero busco el hijo al cual le corresponde la clave
	HijosIterator hi = this->m_hijos.begin();
	ClavesIterator ci = this->m_claves.begin();
	BKDClaveMultiple* regClave = registro.GetClave();

	int dimension = profundidad % regClave->GetDimensiones();

	BKDClave* subclave = regClave->GetSubclave(dimension);

	while (ci != this->m_claves.end() && (*ci)->CompararPorSubclave(*subclave, dimension) != 1)
	{
		ci++;
		hi++;
	}

	delete subclave;
	delete regClave;

	//si por algun motivo llegue al final de la lista de hijos, hay algo mal en el árbol
	if (hi == this->m_hijos.end())
	{
		cerr << "Error: Cantidad de hijos invalida en nodo " << this->m_nro_nodo << endl;
		return false;
	}

	//ahora que tengo al hijo, intento insertar y veo si da overflow
	bool OFhijo = false;
	int numHijo = (*hi);

	BKDNodo* hijo = this->m_manager->GetNodo(numHijo);

	if (hijo == NULL)
	{
		cerr << "Error: El nodo " << this->m_nro_nodo << " apunta a un hijo inexistente: " << numHijo << endl;
		return false;//Datos inconsistentes
	}

	bool res = hijo->InsertarReg(registro, OFhijo, profundidad + 1);

	//si fallo la insercion y no es por overflow, esta roto el arbol
	//if (!res && !OFhijo)
	//	throw "Error al insertar registro";

	//Si hubo overflow al insertar en el hijo, lo resuelvo
	if (OFhijo)
	{
		BKDClaveMultiple* clavePromovida = NULL;
		BKDNodo* hermanoDer = hijo->ResolverOverflow(&clavePromovida);

		if (clavePromovida == NULL)
			cerr << "Error: la clave promovida devuelta por el overflow es NULL. Hijo: " << hijo->GetNumeroNodo() << endl;

		if (hermanoDer != NULL)
		{
			this->m_claves.insert(ci, clavePromovida);
			hi++; //me muevo a la posicion donde corresponde insertar el nuevo hermano
			this->m_hijos.insert(hi, hermanoDer->GetNumeroNodo());

			if (!this->m_manager->GuardarNodo(hijo))
				cerr << "Error al intentar guardar hijo izquierdo: " << hijo->GetNumeroNodo() << endl;

			if (!this->m_manager->GuardarNodo(hermanoDer))
				cerr << "Error al intentar guardar nuevo hijo derecho: " << hermanoDer->GetNumeroNodo() << endl;

			overflow = this->HayOverflow();

			//liberar de la memoria al hermano derecho.
			delete hermanoDer;

			res = true;
		}
		else
			cerr << "Error: fallo el manejo de overflow en nodo: " << this->m_nro_nodo << endl;

	}

	//liberar de la memoria al hijo.
	delete hijo;

	//Solo me guardo a mi mismo en el caso de que no este en overflow, sino me guarda papi :)
	if (!overflow)
		if (!this->m_manager->GuardarNodo(this))
			cerr << "Error al intentar guardar nodo: " << this->m_nro_nodo << endl;

	if (!res)
		cerr << "Error al intentar insertar registro. Nodo: " << this->m_nro_nodo << endl;

	return res;
}

bool BKDNodoInterno::ModificarReg(const BKDRegistro& registro, int profundidad)
{
	return false;
}

bool BKDNodoInterno::EliminarReg(const BKDClaveMultiple& clave, int profundidad)
{
	return false;
}

//Resuelve un overflow en el nodo actual. Devuelve el nuevo hermano creado, y la clave a promover por referencia
//Ante un error, devuelve NULL como retorno
BKDNodo* BKDNodoInterno::ResolverOverflow(BKDClaveMultiple** clavePromovida)
{
	//verifico que estoy en condicion de overflow
	if (!this->HayOverflow())
		return NULL;

	BKDNodoInterno* ret = NULL;
	ret = (BKDNodoInterno*)this->m_manager->AgregarNodo(this->m_nivel);

	if (ret != NULL)
	{
		int aCopiar = this->CantidadAMover();
		ClavesIterator ci = this->m_claves.end();
		HijosIterator hi = this->m_hijos.end();

		//copia
		for(int i = 0; i < aCopiar; i++)
		{
			ci--;
			hi--;
			ret->m_claves.push_front(*ci);
			ret->m_hijos.push_front(*hi);
			this->m_claves.erase(ci);
			this->m_hijos.erase(hi);
		}

		//la clave a promover es la ultima copiada, que no pertenece realmente, asi que la borro
		*clavePromovida = *(ret->m_claves.begin());
		ret->m_claves.pop_front();
	}

	return ret;
}

bool BKDNodoInterno::ClonarNodo(BKDNodo* nodoInt, bool clonarNroNodo)
{
	if (nodoInt == NULL)
		return false;

	if (nodoInt->EsNodoHoja())
		return false; //es un nodo hoja

	BKDNodoInterno* nodo = (BKDNodoInterno*)nodoInt;

	nodo->m_capacidad = this->m_capacidad;
	nodo->m_manager = this->m_manager;
	nodo->m_nivel = this->m_nivel;
	if (clonarNroNodo)
		nodo->m_nro_nodo = this->m_nro_nodo;

	if (nodo->m_claves.size() > 0)
	{
		//recorrer los registros y deletearlos
		for (ClavesIterator it = nodo->m_claves.begin(); it != nodo->m_claves.end(); it++)
		{
			delete *it;
		}

		nodo->m_claves.clear();
	}

	for (ClavesIterator ci = this->m_claves.begin(); ci != this->m_claves.end(); ci++)
	{
		BKDClaveMultiple* clonClave = (*ci)->Clonar();
		if (clonClave == NULL)
			cerr << "Error al intentar clonar clave. Nodo: " << this->m_nro_nodo << endl;

		nodo->m_claves.push_back(clonClave);
		//no borro el clonClave, porque ahora forma parte de las claves del nodo nuevo
	}

	nodo->m_hijos.clear();
	for (HijosIterator hi = this->m_hijos.begin(); hi != this->m_hijos.end(); hi++)
	{
		nodo->m_hijos.push_back(*hi);
	}

	return true;
}

BKDNodoInterno* BKDNodoInterno::PromoverRaiz(BKDManager* manager, BKDNodo* viejaRaiz, BKDClaveMultiple* clavePromovida, int hijoIzquierdo, int hijoDerecho)
{
	if (viejaRaiz == NULL)
		return NULL;

	if (clavePromovida == NULL)
		return NULL;

	BKDNodoInterno* nuevaRaiz = (BKDNodoInterno*)manager->CrearNodoOffline(viejaRaiz->GetNivel() + 1);

	if (nuevaRaiz == NULL)
		return NULL;

	nuevaRaiz->m_nro_nodo = viejaRaiz->GetNumeroNodo();

	nuevaRaiz->m_claves.push_back(clavePromovida->Clonar());
	nuevaRaiz->m_hijos.push_back(hijoIzquierdo);
	nuevaRaiz->m_hijos.push_back(hijoDerecho);

	return nuevaRaiz;
}



//PARA DEBUG
void BKDNodoInterno::DebugPrint(int nivel)
{
	if (nivel > this->m_nivel)
		return;

	// si estoy en el nivel solicitado me imprimo
	if (this->m_nivel == nivel)
	{
		std::cout << "|" << this->m_nro_nodo << "| ";

		HijosIterator hi = this->m_hijos.begin();
		for (ClavesIterator ci = this->m_claves.begin(); ci != this->m_claves.end(); ci++)
		{
			std::cout << " (";
			std::cout << *hi;
			std::cout << ")";
			std::cout << " ['";
			std::cout << (*ci)->ToString();
			std::cout << "']";

			hi++;
		}

		std::cout << " (";
		std::cout << *hi;
		std::cout << ")";

		std::cout << " |";

		return;
	}
	// sino, llamo a mis hijos a que se impriman
	else//nivel < mi nivel
	{
		for (HijosIterator hi = this->m_hijos.begin(); hi != this->m_hijos.end(); hi++)
		{
			BKDNodo* hijo = this->m_manager->GetNodo(*hi);

			if (hijo!= NULL)
			{
				hijo->DebugPrint(nivel);
				delete hijo;
			}

			std::cout << "   ";
		}
	}
}

