/*
 * IndicesManager.h
 *
 *  Created on: May 30, 2012
 *      Author: chompas
 */
#include <map>
#include <string>
#include <utility>

using namespace std;

#ifndef INDICESMANAGER_H_
#define INDICESMANAGER_H_

class IndicesManager {
public:

	//Metodos
	IndicesManager();
	IndicesManager(string filename);

	virtual ~IndicesManager();
	void imprimirCatalogo();
	string obtenerValor(int clave);
	int obtenerClave(string valor);

	//Variables
	map<int, string> catalogo;
};

#endif /* INDICESMANAGER_H_ */
