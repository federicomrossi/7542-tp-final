//  
//  server_receptor.h
//  CLASE RECEPTOR
//  


#ifndef RECEPTOR_H
#define RECEPTOR_H


#include <string>
#include "common_cola.h"
#include "common_mutex.h"
#include "common_lock.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Receptor {
private:

	Cola< std::string > entrada;			// Cola de entrada
	Mutex me;								// Mutex de entrada
	Mutex ms;								// Mutex de salida

public:

	// Constructor
	Receptor();

	// Destructor
	~Receptor();

	// Ingresa un mensaje de entrada en el receptor
	// PRE: 'msg' es la cadena que contiene el mensaje de entrada.
	void ingresarMensajeDeEntrada(std::string msg);

	// Permite obtener un mensaje recibido.
	// POST: Devuelve el primer mensaje de la cola de mensajes entrantes. Al
	// destruir al receptor se devuelve una cadena vacía para permitir seguir
	// con el flujo del programa a los usuarios que se encuentren bloqueados
	// por el método.
	std::string obtenerMensajeDeEntrada();
};

#endif
