//
//  common_utilidades.cpp
//  CLASE UTILIDADES
//


#include "common_utilidades.h"



// Devuelve un string de caracteres alfanumericos aleatorios de 
// tamanio 'longitud'
void Utilidades::randomString(int longitud, std::string &s) {
	// Se limpia la cadena
	s.clear();

	// Tabla de caracteres posibles
	char alphanumerico[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	// Se randomiza el set
	srand(time(NULL));
	int tamanio = sizeof(alphanumerico) - 1;

	for (int i = 0; i < longitud; ++i) 
		s += alphanumerico[rand() % tamanio];
}
