/*
 * Utils.cpp
 *
 *  Created on: Jun 2, 2012
 *      Author: vdiego
 */

#include "Utils.h"
#include <iostream>

using namespace std;

namespace Utils
{
    stringstream errSS(stringstream::out | stringstream::app);

    /*
    void LogError(const ostream& mensaje)
	{
		cerr << endl << errSS.str() << endl << endl;
		errSS.clear();
	}
    */


    void LogError(const ostream& mensaje)
	{
		stringstream* aa = (stringstream*)&mensaje;

		cerr << endl;

		if (aa != &errSS && !errSS.str().empty())
			cerr << errSS.str() << flush;

    	cerr << aa->str() << endl << endl << flush;

    	errSS.str(string());
	}



}


