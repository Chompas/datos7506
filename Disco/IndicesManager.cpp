/*
 * IndicesManager.cpp
 *
 *  Created on: May 30, 2012
 *      Author: chompas
 */

#include "IndicesManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

IndicesManager::IndicesManager() {


}

IndicesManager::IndicesManager(string filename) {

	string txtLine;
	string value;
	int i;
	bool haveValue;
    ifstream infile;
    infile.open (filename.c_str());
    while(getline(infile,txtLine)) // To get you all the lines.
    {
    	istringstream ss(txtLine);
    	haveValue = false;

        while (ss)
        {
        	string s;

        	//Uso punto y coma de separador
        	if (!getline( ss, s, ';' )) break;

        	if (haveValue) {
        		//Luego guardo en el diccionario
        		catalogo[i] = s;
        	} else {
            	//Mi primer valor es la clave, la guardo
            	i = atoi(s.c_str());
            	haveValue = true;
        	}


        }

    }
    infile.close();
}

IndicesManager::~IndicesManager() {

}

void IndicesManager::imprimirCatalogo() {

    for( map<int,string>::iterator ii=catalogo.begin(); ii!=catalogo.end(); ++ii)
    {
        cout << (*ii).first << ": " << (*ii).second << endl;
    }
}

string IndicesManager::obtenerValor(int clave) {

	map<int, string>::const_iterator it = catalogo.find(clave);

	if (catalogo.find(clave) == catalogo.end() ) {
	  // not found
		return "0";
	}
	else {
	  // found
		return it->second;
	}

}

int IndicesManager::obtenerClave(string valor) {

	map<int, string>::const_iterator it;
	int key = -1;

	for (it = catalogo.begin(); it != catalogo.end(); ++it)
	{
	   if (it->second == valor)
	   {
	      key = it->first;
	      break;
	   }
	}

	return key;
}
