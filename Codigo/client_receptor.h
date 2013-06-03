//
//  client_receptor.h
//  CLASE RECEPTOR
//  


#ifndef RECEPTOR_H
#define RECEPTOR_H

#include <string>
#include "common_cola.h"
class Thread;
class Socket;
class Comunicador;




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Receptor : public Thread {
private:

	Socket *socket;							// Socket por el que recibe datos
	Cola< std::string > entrada;			// Cola de entrada
	Comunicador com;						// Comunicador del receptor

public:

	// Constructor
	Receptor(Socket *socket);

	// Destructor
	~Receptor();

	// Permite obtener un mensaje recibido.
	// POST: Devuelve el primer mensaje de la cola de mensajes entrantes.
	std::string obtenerMensajeDeEntrada();

	// Define tareas a ejecutar en el hilo.
	// Se encarga de emitir lo que se encuentre en la cola de salida.
	virtual void run();
};

#endif
