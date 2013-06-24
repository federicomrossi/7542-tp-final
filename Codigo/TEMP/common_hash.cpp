//  
//  common_hash.cpp
//  LIBRERIA HASH
//  
//  Librería de funciones de hash.
//  


#include "common_hash.h"

namespace {
	#define TAM_HASH 32
	#define TAM_HEXA 64
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
	// Se obtiene el hash 
	std::string hash = funcionDeHashBin(cadena);

	return hash;
}

// Aplica la función de hash al string entrante
std::string Hash::funcionDeHashBin(std::string cadena) {
	// Variables auxiliares
	sha256_context ctx;
	uint8 hash[TAM_HASH];

	// Se inicia el algoritmo
	sha256_starts(&ctx);

	uint8 *input = Convertir::htoui(cadena);
	uint32 input_length = cadena.size() / 2;

	// Se le pasa la cadena a calcular el hash
	sha256_update(&ctx, input, input_length);

	// Se devuelve el hash
	sha256_finish(&ctx, hash);

	// Se guarda el char* en un string
	std::string hash_string(Convertir::uitoh(hash, TAM_HASH));
	
	return hash_string;
}

// Aplica la función de hash al char* entrante
std::string Hash::funcionDeHash(const char* cadena, int longitud) {
	// Se guarda la informacion en un string
	std::string s_cadena;
	s_cadena.append(cadena, longitud);

	// Se obtiene el hash 
	std::string hash = funcionDeHashBin(s_cadena);
	
	// Se transforma el hash a una cadena en hexadecimal
	return(Convertir::uitoh((uint8_t*)hash.c_str(), (size_t)TAM_HASH));
}
