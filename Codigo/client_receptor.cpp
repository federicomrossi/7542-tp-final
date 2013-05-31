//
//  client_receptor.h
//  CLASE RECEPTOR
//  

 
#include "common_socket.h"
#include "common_thread.h"
#include "client_receptor.h"





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Receptor::Receptor() { }


// Destructor
Receptor::~Receptor() { }


// Permite obtener un mensaje recibido.
// POST: Devuelve el primer mensaje de la cola de mensajes entrantes.
std::string Receptor::obtenerMensajeDeEntrada() {
	return "nada";
}


// Define tareas a ejecutar en el hilo.
// Se encarga de emitir lo que se encuentre en la cola de salida.
void Receptor::run() {
	
}
