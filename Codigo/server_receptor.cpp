//  
//  server_receptor.h
//  CLASE RECEPTOR
//  


#include "server_receptor.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Receptor::Receptor(Logger *logger) : logger(logger) { }


// Destructor
Receptor::~Receptor() { }


// Ingresa un mensaje de entrada en el receptor
// PRE: 'id' es el identificador de quien ingresa el mensaje; 'msg' es la 
// cadena que contiene el mensaje de entrada.
void Receptor::ingresarMensajeDeEntrada(int id, std::string msg) {
	// Bloqueamos mutex de entrada
	Lock l(this->me);

	// Insertamos mensaje en la cola
	this->entrada.push(std::make_pair(id, msg));
}


// Permite obtener un mensaje recibido.
// POST: devuelve un objeto pair con el primer mensaje de la cola de
// mensajes entrantes. En el objeto pair, el primer miembro contiene el
// identificador de quien envió el mensaje y el segundo miembro contiene
// el mensaje. Al destruir al receptor se devuelve una cadena vacía para 
// permitir seguir con el flujo del programa a los usuarios que se 
// encuentren bloqueados por el método.
std::pair < int, std::string > Receptor::obtenerMensajeDeEntrada() {
	// Bloqueamos mutex de salida
	Lock l(this->ms);

	// Desencolamos mensaje
	return this->entrada.pop_bloqueante();
}
