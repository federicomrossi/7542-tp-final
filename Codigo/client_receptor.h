//
//  client_receptor.h
//  CLASE RECEPTOR
//  


#ifndef RECEPTOR_H
#define RECEPTOR_H

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


class Receptor : public Thread {
private:

	Socket *socket;							// Socket por el que recibe datos
	Cola< std::string > entrada;			// Cola de entrada
	Comunicador com;						// Comunicador del receptor
	Logger *logger;							// Logger de eventos
	std::string clave;						// Clave utilizada para firmar7
											// mensajes
	bool activa;							// Flag de recepción activa

public:

	// Constructor
	Receptor(Socket *socket, Logger *logger, const std::string &clave);

	// Destructor
	~Receptor();

	// Inicia la recepción
	void iniciar();

	// Detiene la recepción
	void detener();

	// Permite obtener un mensaje recibido.
	// POST: Devuelve el primer mensaje de la cola de mensajes entrantes.
	std::string obtenerMensajeDeEntrada();

	// Define tareas a ejecutar en el hilo.
	// Se encarga de recibir y guardar en la cola de entrada.
	virtual void run();

	// Comprueba si la recepción se encuentra activa. Se encontrará activa
	// mientras el socket permanezca activo, lo cual se considera desde que se
	// inicia el objeto con el metodo iniciar(). En caso de cerrarse el socket
	// se devolverá false, mientras que al estar activa la recepción se
	// retornará true.
	bool recepcionActiva();
};

#endif
