//  
//  common_hash.h
//  LIBRERIA HASH
//  
//  Librería de funciones de hash.
//  


#ifndef HASH_H
#define HASH_H


#include <iostream>
#include <string.h>



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Hash {
public:

	// Aplica la función de hash al string entrante
	static std::string funcionDeHash(std::string cadena);
};


#endif
