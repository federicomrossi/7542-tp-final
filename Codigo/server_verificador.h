#ifndef SERVER_VERIFICADOR_H_
#define SERVER_VERIFICADOR_H_

#include <iostream>
#include <fstream>
#include <cstring>
#include "common_mutex.h"
#include "common_lock.h"
#include "common_protocolo.h"
#include "common_parser.h"
#include "common_lista.h"

// Nota: No se pueden utilizar ',' en el nombre de usuario
// Se guardara en el archivo clave + usuario + path carpeta

class Verificador {
public:
	std::ifstream* logins;
	Mutex* mutex;
	
	// Busqueda secuencial
	int buscarCliente(std::string &usuario,	std::string &clave, 
		std::string &carpeta);

public:
	Verificador();
	virtual ~Verificador();

	// Comprueba nombre de usuario y clave de los clientes
	// POST: si la verificación es exitosa, se almacena en 'nombreUsuario' el
	// nombre de usuario del cliente.
	int verificarCliente(std::string &args, std::string& nombreUsuario, 
		std::string &carpeta);
};

#endif /* SERVER_VERIFICADOR_H_ */
