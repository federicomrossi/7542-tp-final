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
	// Devuelve la longitud de la salida del hash
	static int longHash();

	// Aplica la función de hash al string entrante y devuelve el resultado 'imprimible'
	// en caracteres representando solamente numeros hexadecimales en mayuscula
	static std::string funcionDeHash(std::string cadena);

	// Aplica la función de hash al string entrante y devuelve el resultado binario
	static std::string funcionDeHashBin(std::string cadena);

	// Aplica la función de hash al char* entrante
	static std::string funcionDeHash(const char* cadena, int longitud);
};


#endif
