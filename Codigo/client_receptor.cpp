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
Receptor::Receptor(Socket *socket) : socket(socket), com(socket) { }


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
	// Con un mensaje diferente a los posibles esperados por el manejador de 
	// notificaciones, alcanza
	this->entrada.push(COLA_SALIDA_FIN);

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
	// Nos ponemos a la espera de mensajes de entrada
	while(this->isActive()) {
		// Esperamos recepción de mensaje
		std::string mensaje;
		if(this->com.recibir(mensaje) == -1) break;

		// Encolamos el mensaje en cola de entrada
		this->entrada.push(mensaje);
	}
}
