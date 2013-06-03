//
//  client_manejador_de_notificaciones.h
//  CLASE MANEJADORDENOTIFICACIONES
//  


#ifndef MANEJADOR_DE_NOTIFICACIONES_H
#define MANEJADOR_DE_NOTIFICACIONES_H


#include <string>
#include "common_thread.h"
#include "client_receptor.h"
#include "client_sincronizador.h"
#include "client_receptor_de_archivos.h"



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ManejadorDeNotificaciones : public Thread {
private:

	Receptor *receptor;								// Receptor de mensajes
	Sincronizador *sincronizador;					// Sincronizador
	ReceptorDeArchivos *receptorDeArchivos;			// Receptor de archivos

public:

	// Constructor
	ManejadorDeNotificaciones(Receptor *receptor, Sincronizador *sincronizador,
		ReceptorDeArchivos *receptorDeArchivos);

	// Destructor
	~ManejadorDeNotificaciones();

	// Define tareas a ejecutar en el hilo.
	// Routea las notifiaciones y mensajes de entrada
	virtual void run();
};

#endif
