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
};

#endif
