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
Emisor::Emisor() { }


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
// PRE: 'id' es el identificador de quien ingresa el mensaje; 'msg' es la 
// cadena que contiene el mensaje de entrada.
void Emisor::ingresarMensajeDeSalida(int id, std::string msg) {
	// Bloqueamos mutex
	Lock l(this->m);

	// Insertamos mensaje en la cola
	this->salida.push(std::make_pair(id, msg));
}


// Define tareas a ejecutar en el hilo.
// Se encarga de emitir lo que se encuentre en la cola de salida.
void Emisor::run() {

}
