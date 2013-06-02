#ifndef SERVER_VERIFICADOR_H_
#define SERVER_VERIFICADOR_H_

#include <iostream>
#include <fstream>
#include <cstring>
#include "common_mutex.h"
#include "common_lock.h"

namespace {
	#define LONG_USER 128
	#define LONG_CLAVE 128  // Long hash
	#define TAM_BUF 256
}

class Verificador {
public:
	std::fstream* logins;
	std::fstream* new_logins;
	Mutex* mutex;
	
	// Hay que mejorar la busqueda
	int buscarClienteEn(std::fstream* archivo, std::string &usuario,
			std::string &clave);

	/* Hay que hacer que cada cierto tiempo se haga un merge de los archivos
		de login. Se hacen 2 archivos, porque es bastante probable que 
		los que recien crearon usuario se quieran conectar, entonces
		es mas rapido buscar en un archivo chico*/
	void mergeLogins();

public:
	Verificador();
	virtual ~Verificador();
	int agregarCliente(std::string &args);
	int verificarCliente(std::string &args);
};

#endif /* SERVER_VERIFICADOR_H_ */
