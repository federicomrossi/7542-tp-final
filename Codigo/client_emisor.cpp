//  
//  client_emisor.h
//  CLASE EMISOR
//  


#include "client_emisor.h"

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
Emisor::Emisor(Socket *socket, Logger *logger, const std::string &clave) : 
	socket(socket), com(socket), logger(logger), clave(clave) { }


// Destructor
Emisor::~Emisor() { }


// Inicia la emisión
void Emisor::iniciar() {
	this->start();
}


// Detiene la emisión
void Emisor::detener() {
	// Detenemos hilo
	this->stop();

	// Esperamos a que se termine de emitir los mensajes de la cola
	while(!this->salida.vacia());

	// Destrabamos la cola encolando un mensaje de finalización detectable
	this->salida.push(COLA_SALIDA_FIN);
}


// Agrega un mensaje a enviar en la cola de salida del emisor.
// PRE: 'mensaje' es la cadena de texto que se desea enviar
void Emisor::ingresarMensajeDeSalida(std::string mensaje) {
	this->salida.push(mensaje);
}


// Define tareas a ejecutar en el hilo.
// Se encarga de emitir lo que se encuentre en la cola de salida.
void Emisor::run() {
	// Emitimos lo que vaya siendo insertado en la cola de salida. Ante una
	// detención del thread, se seguirá emitiendo hasta vaciar la cola de 
	// salida.
	while(this->isActive() || !this->salida.vacia()) {
		// Tomamos un mensaje de salida
		std::string mensaje = this->salida.pop_bloqueante();

		// Corroboramos si no se ha desencolado el mensaje que marca el fin
		if(mensaje == COLA_SALIDA_FIN) return;

		// Se firma el mensaje
		mensaje = Seguridad::obtenerFirma(mensaje, this->clave) + 
			COMMON_DELIMITER + mensaje;

		// Enviamos mensaje
		if(this->com.emitir(mensaje) == -1) {
			// Mensaje de log
			this->logger->emitirLog("ERROR: Emisor no pudo emitir mensaje.");
			throw "ERROR: No pudo emitirse el mensaje";
		}
	}
}
