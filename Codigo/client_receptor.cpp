//
//  client_receptor.h
//  CLASE RECEPTOR
//  

 
#include "client_receptor.h"

// DEBUG
#include <iostream>
// END DEBUG

namespace {
	const std::string COLA_SALIDA_FIN = "COLA-SALIDA-FIN";
}




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Receptor::Receptor(Socket *socket, Logger *logger, const std::string &clave) :
	socket(socket), com(socket), logger(logger), clave(clave) { }


// Destructor
Receptor::~Receptor() { }


// Inicia la recepción
void Receptor::iniciar() {
	// Iniciamos el hilo
	this->start();
}


// Detiene la recepción
void Receptor::detener() {
	// Detenemos hilo
	this->stop();

	// Destrabamos la cola encolando un mensaje de finalización detectable
	this->entrada.push(COLA_SALIDA_FIN);

	// Cerramos el socket
	this->socket->cerrar();
}


// Permite obtener un mensaje recibido.
// POST: Devuelve el primer mensaje de la cola de mensajes entrantes.
std::string Receptor::obtenerMensajeDeEntrada() {
	return this->entrada.pop_bloqueante();
}


// Define tareas a ejecutar en el hilo.
// Se encarga de emitir lo que se encuentre en la cola de salida.
void Receptor::run() {
	// Variables auxiliares
	std::string firma;
	int delim;

	// Nos ponemos a la espera de mensajes de entrada
	while(this->isActive()) {
		// Esperamos recepción de mensaje
		std::string mensaje;
		if(this->com.recibir(mensaje) == -1) {
			// Mensaje de log
			this->logger->emitirLog("RECEPTOR: Se desconectó el servidor.");
			
			// DEBUG
			std::cout << "SE DESCONECTO EL SERVIDOR" << std::endl;
			// END DEGUG

			break;
		}
		// Se separa la firma del mensaje
		delim = mensaje.find(COMMON_DELIMITER);
		firma = mensaje.substr(0, delim);
		mensaje = mensaje.substr(delim + 1);

		// Se comprueba la validez de la firma
		if (Seguridad::firmaValida(mensaje, this->clave, firma)) {
			// Encolamos el mensaje en cola de entrada
			this->entrada.push(mensaje);
		}
		else {
			// Enviar mensaje de desconexion

			// Detener ejecucion
			this->detener();
		}
	}
}
