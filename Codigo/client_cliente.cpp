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




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Cliente::Cliente(std::string nombreHost, int puerto) : 
	puerto(puerto), nombreHost(nombreHost) { }


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

	ManejadorArchivos ma;
	std::cout << ma.obtenerArchivo("cliente/archivo.txt") << std::endl 
		<< std::endl;

	// // Variables de procesamiento
	// std::string instruccion;
	// std::string args;

	// // Recibimos respuesta del servidor
	// if(comunicador.recibir(instruccion, args) == -1) return;
	
	// // Caso en que no hay trabajo para realizar
	// if(instruccion == S_NO_JOB_PART) {
	// 	// Desconectamos el socket y salimos
	// 	this->socket.cerrar();
	// 	return;
	// }
	// else if (instruccion == S_JOB_PART) {
	// 	// Variables auxiliares para datos
	// 	std::string msgEncriptado, numParte;
	// 	int numDig, claveIni, claveFin;
	// 	std::stringstream args_stream(args);

	// 	// Parseamos y obtenemos datos del argumento
	// 	args_stream >> msgEncriptado >> numParte >> numDig >> claveIni 
	// 		>> claveFin;

	// 	// Probamos el rango de claves indicado por el servidor
	// 	procesarClaves(msgEncriptado, numDig, claveIni, claveFin);

	// 	// Avisamos al servidor la finalización del trabajo
	// 	if(comunicador.emitir(C_JOB_PART_FINISHED, numParte) == -1) return;
	// }
	// else
	// 	std::cerr << "Mensaje inválido del servidor" << std::endl;

	// Mensaje de log
	std::cout << "Cerrando conexión... ";
    std::cout.flush();

	// Desconectamos el socket
	this->socket.cerrar();

	// Mensaje de log
	std::cout << "DESCONECTADO" << std::endl;
	std::cout.flush();
}
