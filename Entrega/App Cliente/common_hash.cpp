//  
//  common_hash.cpp
//  LIBRERIA HASH
//  
//  Librería de funciones de hash.
//  


#include "common_hash.h"

namespace {
	#define TAM_HASH 20
	#define TAM_HEXA 42
}




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Devuelve la longitud de la salida del hash
int Hash::longHash() {
	return TAM_HASH;
}

// Aplica la función de hash al string entrante
std::string Hash::funcionDeHash(std::string cadena) {
	// Variables auxiliares
	unsigned char hash[TAM_HASH];
	char hex_hash[TAM_HEXA];
	std::string aux;

	// Se inicializan
	memset(hash, 0, TAM_HASH);
	memset(hex_hash, 0, TAM_HEXA);

	// Se calcula
	sha1::calc(cadena.c_str(), cadena.length(), hash);

	// Se transforma a hexa
	sha1::toHexString(hash, hex_hash);
	
	aux = hex_hash;

	return aux;
}

// Aplica la función de hash al string entrante
std::string Hash::funcionDeHashBin(std::string cadena) {
	// Variables auxiliares
	unsigned char hash[TAM_HASH];
	std::string aux;

	// Se inicializan
	memset(hash, 0, TAM_HASH);

	// Se calcula
	sha1::calc(cadena.c_str(), cadena.length(), hash);

	aux = (char*)hash;
	
	return aux;
}

// Aplica la función de hash al char* entrante
std::string Hash::funcionDeHash(const char* cadena, int longitud) {
	// Variables auxiliares
	unsigned char hash[TAM_HASH];
	char hex_hash[TAM_HEXA];
	std::string aux;

	// Se inicializan
	memset(hash, 0, TAM_HASH);
	memset(hex_hash, 0, TAM_HEXA);

	// Se calcula
	sha1::calc(cadena, longitud, hash);

	// Se transforma a hexa
	sha1::toHexString(hash, hex_hash);

	aux = hex_hash;
	
	return aux;
}
