#ifndef __ARBOLBMAS_H__
#define __ARBOLBMAS_H__

#include <stdio.h>
#include <string.h>
#include "Hoja.h"
#include "Interno.h"

class ArbolBMas{
    private:
        Nodo*	raiz; 			
        int 	cantNiveles; 	
        int		capaHoja;
        int		capaInterno;
        
    public:
        ArbolBMas(int capaHoja, int capaInternos);
                
        void agregarRegistro(std::string registro);        
        std::string obtenerRegistro(const std::string clave);
        bool estaVacio();
        
        virtual ~ArbolBMas();
             
};

#endif // __ARBOLBMAS_H__
