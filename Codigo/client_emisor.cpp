//  
//  client_emisor.h
//  CLASE EMISOR
//  


#include "common_thread.h"
#include "common_socket.h"
#include "common_comunicador.h"
#include "client_emisor.h"





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Emisor::Emisor(Socket *socket) : socket(socket), com(socket) { }


// Destructor
Emisor::~Emisor() {
	// Detenemos hilo
	this->stop();

	// Esperamos a que se termine de emitir los mensajes de la cola
	while(!this->salida.vacia());

	// Destrabamos la cola
	this->salida.desbloquear();

	// Forzamos el cierre del socket y destrabamos espera de recepcion de datos
	try {
		this->socket->cerrar();
	}
	// Ante una eventual detención abrupta, previa a la inicialización del
	// socket, lanzará un error que daremos por obviado.
	catch(...) { }
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

		// Caso en que se ha desbloqueado el desencolado ante una petición
		// de detención
		if(this->salida.vacia() && mensaje == "") return;

		// Enviamos mensaje
		if(this->com.emitir(mensaje) == -1)
			throw "ERROR: No pudo emitirse el mensaje";
	}
}
