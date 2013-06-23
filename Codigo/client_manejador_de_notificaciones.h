//
//  client_manejador_de_notificaciones.h
//  CLASE MANEJADORDENOTIFICACIONES
//  


#ifndef MANEJADOR_DE_NOTIFICACIONES_H
#define MANEJADOR_DE_NOTIFICACIONES_H


#include <string>
#include "common_thread.h"
#include "common_logger.h"
#include "client_receptor.h"
#include "client_inspector.h"
#include "client_receptor_de_archivos.h"

//DEBUG
#include <iostream>
//FIN DEBUG




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ManejadorDeNotificaciones : public Thread {
private:

	Receptor *receptor;								// Receptor de mensajes
	Inspector *inspector;							// Inspector
	ReceptorDeArchivos *receptorDeArchivos;			// Receptor de archivos
	Logger *logger;									// Logger de eventos

public:

	// Constructor
	ManejadorDeNotificaciones(Receptor *receptor, Inspector *inspector,
		ReceptorDeArchivos *receptorDeArchivos, Logger *logger);

	// Destructor
	~ManejadorDeNotificaciones();

	// Define tareas a ejecutar en el hilo.
	// Routea las notifiaciones y mensajes de entrada
	virtual void run();
};

#endif
