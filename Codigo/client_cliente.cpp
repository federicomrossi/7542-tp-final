//
//  client_cliente.h
//  CLASE CLIENTE
//   


#include <iostream>
#include <sstream>

#include "common_comunicador.h"
#include "common_convertir.h"
#include "common_logger.h"

#include "client_emisor.h"
#include "client_receptor.h"
#include "client_manejador_de_archivos.h"
#include "client_inspector.h"
#include "client_cliente.h"






/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Cliente::Cliente(std::string nombreHost, int puerto, Logger *logger) : 
	puerto(puerto), nombreHost(nombreHost), logger(logger) {
	// Creamos socket
	this->socket.crear();

	// Creamos comunicador (SACAR, CREA EN EMISOR Y RECEPTOR)
	this->com = new Comunicador(&this->socket);
	
	// Creamos el inspector
}


// Destructor
Cliente::~Cliente() {
	this->socket.cerrar();	
}


// Se conecta con el servidor
int Cliente::conectar() {

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
		return 0;
	}

	// Mensaje de log
	std::cout << "CONECTADO" << std::endl;
	std::cout.flush();

	// Se conecto correctamente
	return 1;
}


// Se desconecta del servidor
void Cliente::desconectar() {
	// Mensaje de log
	std::cout << "Cerrando conexión... ";
    std::cout.flush();

	// Desconectamos el socket
	this->socket.cerrar();

	// Mensaje de log
	std::cout << "DESCONECTADO" << std::endl;
	std::cout.flush();
}


// Inicia sesion con usuario existente
int Cliente::iniciarSesion(std::string &usuario, std::string &clave) {
	
	// Mensaje de log
	std::cout << "Emitiendo solicitud de LOGIN... " << std::endl;
    std::cout.flush();
	
	// Se preparan los argumentos
	std::string mensaje = usuario + '-' + clave;	

	// Enviamos petición de inicio de sesion
	if(this->com->emitir(C_LOGIN_REQUEST, mensaje) == -1) {
		return -1;
	}

	// Se obtiene respuesta del servidor
	std::string args;
	if(this->com->recibir(mensaje, args) == -1) {
		return -1;
	}
	
	if (mensaje == S_LOGIN_OK) {
		std::cout << "Inicio de sesion exitoso" << std::endl;
		std::cout.flush();
		return 1;
	}
	if (mensaje == S_LOGIN_FAIL) {
		std::cout << "Inicio de sesion fallo, compruebe nombre de usuario y contrasenia" << std::endl;
		std::cout.flush();
		return 0;
	}
	return -1;
}


// Mantiene la comunicación con el servidor.
void Cliente::ejecutar() {
	// Se conecta al servidor
	conectar();

	// Inicia sesion
	std::string usuario;
	std::string clave;
	// std::string usuario = "Fiona";
	// std::string clave = "456";
	
	while((usuario == "" || clave == "") || iniciarSesion(usuario, clave) != 1)
	{
		std::cout << "Usuario: ";
		std::cout.flush();
		getline(std::cin, usuario);
		std::cout << "Contraseña: ";
		std::cout.flush();
		getline(std::cin, clave);
		std::cout << std::endl;
	}
	// FIN Inicio sesion


	// Creamos los modulos que hacen parte del cliente.
	Emisor emisor(&this->socket);
	Receptor receptor(&this->socket);
	ManejadorDeArchivos manejadorDeArchivos("cliente");


	// DEBUG
	// Mensaje de log
	this->logger->emitirLog("Se emite mensaje inicial");
	std::cout << "Emitiendo mensaje inicial... ";
    std::cout.flush();
	
	// Enviamos petición de parte de trabajo
	if(this->com->emitir(COMMON_SEND_FILE) == -1) return;

	// Mensaje de log
	std::cout << "OK" << std::endl;
	std::cout.flush();

	int INTERVALO = 5;
    Inspector i(INTERVALO);
    i.iniciar();

   
	std::string comando;
	while(comando != "s")
		getline(std::cin, comando);
	// END DEBUG

	i.detener();
    i.join();

	// Se desconecta del servidor
	desconectar();
}
