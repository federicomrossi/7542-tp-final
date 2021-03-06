//
//  client_emisor.h
//  CLASE EMISOR
//  


#ifndef EMISOR_H
#define EMISOR_H

#include <string>
#include "common_cola.h"
#include "common_thread.h"
#include "common_socket.h"
#include "common_comunicador.h"
#include "common_logger.h"
#include "common_seguridad.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Emisor : public Thread {
private:

	Socket *socket;							// Socket por el que envía datos
	Cola< std::string > salida;				// Cola de salida
	Comunicador com;						// Comunicador del emisor
	Logger *logger;							// Logger de eventos
	std::string clave;						// Clave utilizada para firmar 
											// mensajes
	bool activa;							// Flag de emisión activa


public:

	// Constructor
	Emisor(Socket *socket, Logger *logger, const std::string &clave);

	// Destructor
	~Emisor();

	// Inicia la emisión
	void iniciar();

	// Detiene la emisión
	void detener();

	// Agrega un mensaje a enviar en la cola de salida del emisor.
	// PRE: 'mensaje' es la cadena de texto que se desea enviar
	void ingresarMensajeDeSalida(std::string mensaje);

	// Define tareas a ejecutar en el hilo.
	// Se encarga de emitir lo que se encuentre en la cola de salida.
	virtual void run();

	// Comprueba si la emisión se encuentra activa. Se encontrará activa
	// mientras el socket permanezca activo, lo cual se considera desde que se
	// inicia el objeto con el metodo iniciar(). En caso de cerrarse el socket
	// se devolverá false, mientras que al estar activa la recepción se
	// retornará true.
	bool emisionActiva();
};

#endif
