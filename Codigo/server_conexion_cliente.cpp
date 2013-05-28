//
//  common_conexion_cliente.h
//  CLASE CONEXIONCLIENTE
//   

#include <iostream>
#include <sstream>
#include "server_conexion_cliente.h"
#include "common_comunicador.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
// PRE: 's' es un socket para la comunicación con el cliente; 'id' es 
// número de cliente que se le ha sido asignado por el servidor; 'serv' es
// una referencia al servidor al que pertenece la conexión.
//ConexionCliente::ConexionCliente(Socket *s, ControladorDeTareas *controlador)
//	: socket(s), controlador(controlador) { }
ConexionCliente::ConexionCliente(Socket *s) : socket(s) { }


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
	std::string instruccion;
	std::string args;
	// std::stringstream msg_in;
	// std::string msg_tarea;

	// Mensaje de log
	std::cout << "Esperando mensaje del cliente... ";
    std::cout.flush();

	// Esperamos hasta recibir el mensaje correcto
	while(instruccion != COMMON_SEND_FILE || !this->isActive())
		if(comunicador.recibir(instruccion, args) == -1) return;

	// Mensaje de log
	std::cout << "RECIBIDO" << std::endl;
    std::cout.flush();

	// if(!this->controlador->obtenerIndicacion(msg_tarea)) {
	// 	// No hay tarea asignada
	// 	comunicador.emitir(msg_tarea);
	// 	this->socket->cerrar();
	// 	return;
	// }

	// // Enviamos la parte del trabajo correspondiente
	// if(comunicador.emitir(msg_tarea) == -1) return;
	

	// // Nos ponemos a la espera de posibles claves o de indicación de
	// // finalización de tarea por parte del cliente
	// while(this->isActive()) {
	// 	// Recibimos mensaje
	// 	if(comunicador.recibir(instruccion, args) == -1) break;

	// 	// Caso en que se recibe una posible clave
	// 	if(instruccion == C_POSSIBLE_KEY)
	// 		this->controlador->ingresarClave(args);
	// 	else if (instruccion == C_JOB_PART_FINISHED) {
	// 		this->controlador->clienteTerminoTarea();
	// 		break;
	// 	}
	// }

	// Cerramos conexión
	this->socket->cerrar();
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
