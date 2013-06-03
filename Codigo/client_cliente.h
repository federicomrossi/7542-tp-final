//
//  client_cliente.h
//  CLASE CLIENTE
//  


#ifndef CLIENTE_H
#define CLIENTE_H


#include "common_socket.h"
class Comunicador;



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Cliente {
private:

	Socket socket;					// Socket con el que se comunica
	int puerto;						// Puerto de conexión.
	std::string nombreHost;			// Nombre del host de conexión
	Comunicador *com;				// Comunicador del cliente


public:

	// Constructor
	Cliente(std::string nombreHost, int puerto);

	// Destructor
	~Cliente();

	// Se conecta con el servidor. 
	// POST: Devuelve 1 si lo logra sin problemas y 0 sino
	int conectar();

	// Se desconecta del servidor
	void desconectar();

	// Inicia sesion con usuario existente
	int iniciarSesion(std::string &usuario, std::string &clave);

	// Mantiene la comunicación con el servidor.
	void ejecutar();
};

#endif
