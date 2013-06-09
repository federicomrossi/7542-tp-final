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
#include "common_sha1.h"
#include "common_convertir.h"



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Hash {
public:

	// Aplica la función de hash al string entrante
	static std::string funcionDeHash(std::string cadena);


	// Aplica la función de hash al char* entrante
	static std::string funcionDeHash(const char* cadena, int longitud);
};


#endif
