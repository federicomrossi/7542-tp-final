//  
//  server_emisor.h
//  CLASE EMISOR
//  


#include "server_emisor.h"



namespace {
	const std::string COLA_SALIDA_FIN = "COLA-SALIDA-FIN";
}




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Emisor::Emisor(Lista< ConexionCliente* > *listaConexiones, Logger *logger,
	const std::string &clave) : 
	listaConexiones(listaConexiones), logger(logger), clave(clave) { }


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
	this->salida.push(std::make_pair(0, COLA_SALIDA_FIN));
}


// Ingresa un mensaje de entrada en el receptor
// PRE: 'id' es el identificador de quien ingresa el mensaje. Si se desea
// enviar un mensaje a todas las conexiones, el id debe ser 0 (cero); 'msg' es
// la cadena que contiene el mensaje de entrada.
void Emisor::ingresarMensajeDeSalida(int id, std::string msg) {
	// Bloqueamos mutex
	Lock l(this->m);

	// Insertamos mensaje en la cola
	this->salida.push(std::make_pair(id, msg));
}


// Define tareas a ejecutar en el hilo.
// Se encarga de emitir lo que se encuentre en la cola de salida.
void Emisor::run() {
	// Variables auxiliares
	std::string mensajeFirmado;

	// Emitimos lo que vaya siendo insertado en la cola de salida. Ante una
	// detención del thread, se seguirá emitiendo hasta vaciar la cola de 
	// salida.
	while(this->isActive() || !this->salida.vacia()) {
		// Tomamos un mensaje de salida
		std::pair < int, std::string > mensaje = this->salida.pop_bloqueante();

		// Corroboramos si no se ha desencolado el mensaje que marca el fin
		if(mensaje.second == COLA_SALIDA_FIN) return;

		// Se firma el mensaje
		mensajeFirmado = Seguridad::obtenerFirma(mensaje.second, this->clave) +
			COMMON_DELIMITER + mensaje.second;

		// Caso en que se debe enviar el mismo mensaje a todos los clientes
		if(mensaje.first == 0) {
			// Iteramos sobre la lista y enviamos el mensaje uno a uno
			for(size_t i = 0; i < this->listaConexiones->tamanio(); i++)
				(*this->listaConexiones)[i]->enviarMensaje(mensajeFirmado);
		}
		// Caso en que se debe enviar el mensaje a un único cliente
		else {
			// Iteramos sobre la lista hasta encontrar al destinatario correcto
			for(size_t i = 0; i < this->listaConexiones->tamanio(); i++) {
				ConexionCliente *cc = (*this->listaConexiones)[i];
				
				// Comparamos identificadores para ver si es el cliente deseado
				if(cc->id() == mensaje.first){
					cc->enviarMensaje(mensajeFirmado);
					break;
				}
			}
		}
	}
}
