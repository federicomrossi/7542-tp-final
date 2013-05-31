//  
//  client_emisor.h
//  CLASE EMISOR
//  


#include "common_socket.h"
#include "common_thread.h"
#include "client_emisor.h"





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Emisor::Emisor() { }


// Destructor
Emisor::~Emisor() { }


// Agrega un mensaje a enviar en la cola de salida del emisor.
// PRE: 'mensaje' es la cadena de texto que se desea enviar
void Emisor::ingresarMensajeDeSalida(std::string mensaje) {

}


// Define tareas a ejecutar en el hilo.
// Se encarga de emitir lo que se encuentre en la cola de salida.
void Emisor::run() {
	
}
