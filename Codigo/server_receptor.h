//  
//  server_receptor.h
//  CLASE RECEPTOR
//  


#ifndef RECEPTOR_H
#define RECEPTOR_H


#include <string>
#include <utility>
#include "common_cola.h"
#include "common_mutex.h"
#include "common_lock.h"
#include "common_logger.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Receptor {
private:

	Cola< std::pair < int, std::string > > entrada;			// Cola de entrada
	Mutex me;												// Mutex de entrada
	Mutex ms;												// Mutex de salida
	Logger *logger;											// Logger de eventos

public:

	// Constructor
	Receptor(Logger *logger);

	// Destructor
	~Receptor();

	// Ingresa un mensaje de entrada en el receptor
	// PRE: 'id' es el identificador de quien ingresa el mensaje; 'msg' es la 
	// cadena que contiene el mensaje de entrada.
	void ingresarMensajeDeEntrada(int id, std::string msg);

	// Permite obtener un mensaje recibido.
	// POST: devuelve un objeto pair con el primer mensaje de la cola de
	// mensajes entrantes. En el objeto pair, el primer miembro contiene el
	// identificador de quien envió el mensaje y el segundo miembro contiene
	// el mensaje. Al destruir al receptor se devuelve una cadena vacía para 
	// permitir seguir con el flujo del programa a los usuarios que se 
	// encuentren bloqueados por el método.
	std::pair < int, std::string > obtenerMensajeDeEntrada();
};

#endif
