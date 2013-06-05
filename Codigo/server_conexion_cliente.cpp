//
//  common_conexion_cliente.h
//  CLASE CONEXIONCLIENTE
//   

#include <iostream>
#include <sstream>
#include "server_conexion_cliente.h"





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
// PRE: 's' es un socket para la comunicación con el cliente; 'id' es 
// número de cliente que se le ha sido asignado por el servidor; 'serv' es
// una referencia al servidor al que pertenece la conexión.
//ConexionCliente::ConexionCliente(Socket *s, ControladorDeTareas *controlador)
//	: socket(s), controlador(controlador) { }
ConexionCliente::ConexionCliente(Socket *s, Verificador* v) : socket(s),
	verificador(v) {}


// Destructor
ConexionCliente::~ConexionCliente() { 
	// Liberamos memoria utilizada por el socket
	delete this->socket;
}

// Define tareas a ejecutar en el hilo.
void ConexionCliente::run() {
	// Creamos el comunicador para enviar y recibir mensajes
	Comunicador comunicador(this->socket);

	// Variables de procesamiento
	std::string mensaje;

	// Mensaje de log
	std::cout << "Esperando mensaje del cliente... ";
    std::cout.flush();

	// Si el inicio de sesión falló, cerramos conexión con cliente
	if(this->inicioSesion(comunicador) != 1) return;
	// ACÁ FALTA AGREGAR LA SOLICITUD PARA SER REMOVIDO DE LA LISTA DE CLIENTES

	// Sincronización
	while(this->isActive()) {
		// Esperamos a recibir mensaje
		if(comunicador.recibir(mensaje) == -1) break;

		std::cout << "ENTRANTE: " <<  mensaje << std::endl;
		std::cout.flush();
	}

	std::cout << "TERMINO" << std::endl;
}


// Detiene la conexión con el cliente. No debe utilizarse el método stop()
// para detener, sino este mismo en su lugar.
void ConexionCliente::detener() {
	// Detenemos hilo
	this->stop();
	
	// Forzamos el cierre del socket y destrabamos espera de recepcion de datos
	try {
		this->socket->cerrar();
	}
	// Ante una eventual detención abrupta, previa a la inicialización del
	// socket, lanzará un error que daremos por obviado.
	catch(...) { } 
}




/*
 * IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */
 

// Espera inicio sesion
int ConexionCliente::inicioSesion(Comunicador& comunicador) {
	// Variables de procesamiento
	std::string instruccion;
	std::string args;
	
	// Se recibe la instruccion
	if(comunicador.recibir(instruccion, args) == -1) 
		return -1;

	// Se debe crear nuevo usuario
	if (instruccion == C_LOGIN_REQUEST) {
		if (verificador->verificarCliente(args) == 1) {  // OK
			comunicador.emitir(S_LOGIN_OK);
			std::cout << "Inicio de sesion OK" << std::endl;
			std::cout.flush();	
			return 1;
		}
		else {
			comunicador.emitir(S_LOGIN_FAIL);
			std::cout << "Inicio de sesion fallo" << std::endl;
			std::cout.flush();
			return 0;
		}
	}
	return -1;
}
