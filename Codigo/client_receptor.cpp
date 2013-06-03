//
//  client_receptor.h
//  CLASE RECEPTOR
//  

 
#include "client_receptor.h"





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Receptor::Receptor(Socket *socket) : socket(socket), com(socket) { }


// Destructor
Receptor::~Receptor() {
	// Detenemos hilo
	this->stop();

	// Desbloqueamos la cola de salida
	this->entrada.desbloquear();

	// Forzamos el cierre del socket y destrabamos espera de recepción de datos
	try {
		this->socket->cerrar();
	}
	// Ante una eventual detención abrupta, previa a la inicialización del
	// socket, lanzará un error que daremos por obviado.
	catch(...) { }
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
