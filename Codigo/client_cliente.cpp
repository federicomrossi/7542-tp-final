//
//  client_cliente.h
//  CLASE CLIENTE
//   


#include <iostream>
#include <sstream>
#include "common_convertir.h"
#include "common_comunicador.h"
#include "client_cliente.h"

#include "client_manejador_archivos.h"
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

	// DEBUG
	std::string comando;
	while(comando != "s")
		getline(std::cin, comando);
	// END DEBUG

	// Mensaje de log
	std::cout << "Emitiendo mensaje inicial... ";
    std::cout.flush();
	
	// Enviamos petición de parte de trabajo
	if(comunicador.emitir(COMMON_SEND_FILE) == -1) return;

	// Mensaje de log
	std::cout << "OK" << std::endl;
	std::cout.flush();

	std::cout << std::endl << std::endl << "CONTENIDO DE ARCHIVO" 
		<< std::endl << std::endl;

	// DEBUG
	ManejadorArchivos ma("cliente");
	std::cout << ma.obtenerContenidoArchivo("/run/media/federicorossi/Archivos/Webserver/www/fiuba/75.42/TP5/Servidor/archivo.txt") 
		<< std::endl << std::endl;

	std::list<std::string> l = ma.obtenerArchivosDeDirectorio();

	for (std::list< std::string>::iterator it = l.begin(); it != l.end(); ++it)
    	std::cout << *it << std::endl;

    l.sort();

    std::cout << std::endl;
    for (std::list< std::string>::iterator it = l.begin(); it != l.end(); ++it)
    	std::cout << *it << std::endl;

    Inspector i(1);
    i.start();
    i.join();
	// END DEBUG


	// Mensaje de log
	std::cout << "Cerrando conexión... ";
    std::cout.flush();

	// Desconectamos el socket
	this->socket.cerrar();

	// Mensaje de log
	std::cout << "DESCONECTADO" << std::endl;
	std::cout.flush();
}
