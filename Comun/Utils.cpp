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
    stringstream dbgSS(stringstream::out | stringstream::app);
    bool debugMode = false;

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
		cerr << " *** ";

		if (aa != &errSS && !errSS.str().empty())
			cerr << errSS.str() << flush;

    	cerr << aa->str() << " *** " << endl << endl << flush;

    	errSS.str(string());
	}

    void LogDebug(const ostream& mensaje)
    	{
    		if (!debugMode)
    			return;

    		stringstream* aa = (stringstream*)&mensaje;

    		cout << endl;

    		if (aa != &dbgSS && !dbgSS.str().empty())
    			cout << dbgSS.str() << flush;

        	cout << aa->str() << endl << endl << flush;

        	dbgSS.str(string());
    	}



}


