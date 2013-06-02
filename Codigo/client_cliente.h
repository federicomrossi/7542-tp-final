//
//  client_cliente.h
//  CLASE CLIENTE
//  


#ifndef CLIENTE_H
#define CLIENTE_H


#include "common_socket.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Cliente {
private:

	Socket socket;						// Socket con el que se comunica
	int puerto;							// Puerto de conexión.
	std::string nombreHost;				// Nombre del host de conexión

public:

	// Constructor
	Cliente(std::string nombreHost, int puerto);

	// Destructor
	~Cliente();

	// Mantiene la comunicación con el servidor.
	void ejecutar();

	/* Se conecta con el servidor. Devuelve 1 si lo logra sin problemas
	   y 0 sino */
	int conectar();

	// Se desconecta del servidor
	void desconectar();

	// Inicia sesion con usuario existente
	int iniciarSesion(std::string &usuario, std::string &clave);

	// Crea un nuevo usuario
	int crearUsuario(std::string &usuario, std::string &clave);
};

#endif
