//  
//  common_hash.cpp
//  LIBRERIA HASH
//  
//  Librería de funciones de hash.
//  


#include "common_hash.h"

namespace {
	#define TAM_HASH 20
	#define TAM_HEXA 41
}


/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/

// Aplica la función de hash al string entrante
std::string Hash::funcionDeHash(std::string cadena) {
	unsigned char hash[TAM_HASH];
	char hash_hexa[TAM_HEXA];

	// Se inicializan las variables
	memset(hash, '\0', TAM_HASH);
	memset(hash_hexa, '\0', TAM_HEXA);

	// Se calcula el hash con el algoritmo sha1
	sha1::calc((void*)cadena.c_str(), cadena.length(), hash);

	// Se transforma el hash a una cadena en hexadecimal
	return(Convertir::uitoh(hash, TAM_HASH));
}

// Aplica la función de hash al char* entrante
std::string Hash::funcionDeHash(const char* cadena, int longitud) {
	unsigned char hash[TAM_HASH];
	char hash_hexa[TAM_HEXA];

	// Se inicializan las variables
	memset(hash, '\0', TAM_HASH);
	memset(hash_hexa, '\0', TAM_HEXA);

	// Se calcula el hash con el algoritmo sha1
	sha1::calc((void*)cadena, (unsigned int)longitud, hash);

	// Se transforma el hash a una cadena en hexadecimal
	return(Convertir::uitoh(hash, TAM_HASH));
}
