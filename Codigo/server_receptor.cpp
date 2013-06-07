//  
//  server_receptor.h
//  CLASE RECEPTOR
//  


#include "server_receptor.h"



namespace {
	const std::string COLA_MENSAJE_FIN = "";
}



/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Receptor::Receptor() { }


// Destructor
Receptor::~Receptor() {
	// Destrabamos la cola encolando un mensaje de finalización detectable
	this->ingresarMensajeDeEntrada(COLA_MENSAJE_FIN);
}


// Ingresa un mensaje de entrada en el receptor
// PRE: 'msg' es la cadena que contiene el mensaje de entrada.
void Receptor::ingresarMensajeDeEntrada(std::string msg) {
	// Bloqueamos mutex de entrada
	Lock l(this->me);

	// Insertamos mensaje en la cola
	this->entrada.push(msg);
}


// Permite obtener un mensaje recibido.
// POST: Devuelve el primer mensaje de la cola de mensajes entrantes. Al
// destruir al receptor se devuelve una cadena vacía para permitir seguir
// con el flujo del programa a los usuarios que se encuentren bloqueados
// por el método.
std::string Receptor::obtenerMensajeDeEntrada() {
	// Bloqueamos mutex de salida
	Lock l(this->ms);

	// Desencolamos mensaje
	return this->entrada.pop_bloqueante();
}
