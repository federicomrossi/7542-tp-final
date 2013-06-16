#ifndef SERVER_VERIFICADOR_H_
#define SERVER_VERIFICADOR_H_

#include <iostream>
#include <fstream>
#include <cstring>
#include "common_mutex.h"
#include "common_lock.h"
#include "common_protocolo.h"

namespace {
	#define LONG_USER 128
	#define LONG_CLAVE 128  // Long hash
	#define TAM_BUF (LONG_USER + LONG_CLAVE)
}

//Nota: No se pueden utilizar '-' en el nombre de usuario por ahora
//Mas adelante se guardara en el archivo clave + usuario (long necesaria
// y sin requerimientos para el nombre)

class Verificador {
public:
	std::fstream* logins;
	Mutex* mutex;
	
	// Busqueda secuencial
	int buscarCliente(std::string &usuario,	std::string &clave);

public:
	Verificador();
	virtual ~Verificador();

	// Comprueba nombre de usuario y clave de los clientes
	// POST: si la verificación es exitosa, se almacena en 'nombreUsuario' el
	// nombre de usuario del cliente.
	int verificarCliente(std::string &args, std::string& nombreUsuario);
};

#endif /* SERVER_VERIFICADOR_H_ */
