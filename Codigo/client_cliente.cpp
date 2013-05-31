//
//  client_cliente.h
//  CLASE CLIENTE
//   


#include <iostream>
#include <sstream>
#include "common_convertir.h"
#include "common_comunicador.h"
#include "client_cliente.h"


#include "client_inspector.h"






/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Cliente::Cliente(std::string nombreHost, int puerto) : 
	puerto(puerto), nombreHost(nombreHost) 
{
	// Creamos el inspector

}


// Destructor
Cliente::~Cliente() {
	this->socket.cerrar();	
}


// Mantiene la comunicación con el servidor.
void Cliente::ejecutar() {
	// Creamos socket
	this->socket.crear();

	// Mensaje de log
	std::cout << "Conectando con " << this->nombreHost << " en el puerto " 
		<< this->puerto << "... ";
    std::cout.flush();

	try {
		// Conectamos el socket
		this->socket.conectar(nombreHost, puerto);
	}
	catch(char const * e) {
		std::cerr << e << std::endl;
		return;
	}

	// Mensaje de log
	std::cout << "CONECTADO" << std::endl;
	std::cout.flush();
	
	// Creamos el comunicador para enviar y recibir mensajes
	Comunicador comunicador(&this->socket);

	// Mensaje de log
	std::cout << "Emitiendo mensaje inicial... ";
    std::cout.flush();
	
	// Enviamos petición de parte de trabajo
	if(comunicador.emitir(COMMON_SEND_FILE) == -1) return;

	// Mensaje de log
	std::cout << "OK" << std::endl;
	std::cout.flush();

	int INTERVALO = 5;
    Inspector i(INTERVALO);
    i.iniciar();

    // DEBUG
	std::string comando;
	while(comando != "s")
		getline(std::cin, comando);
	// END DEBUG

	i.detener();
    i.join();

	// Mensaje de log
	std::cout << "Cerrando conexión... ";
    std::cout.flush();

	// Desconectamos el socket
	this->socket.cerrar();

	// Mensaje de log
	std::cout << "DESCONECTADO" << std::endl;
	std::cout.flush();
}
