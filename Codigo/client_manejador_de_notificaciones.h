//
//  client_manejador_de_notificaciones.h
//  CLASE MANEJADORDENOTIFICACIONES
//  


#ifndef MANEJADOR_DE_NOTIFICACIONES_H
#define MANEJADOR_DE_NOTIFICACIONES_H


#include <string>
class Thread;



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ManejadorDeNotificaciones : public Thread {
private:


public:

	// Constructor
	ManejadorDeNotificaciones();

	// Destructor
	~ManejadorDeNotificaciones();

	// Define tareas a ejecutar en el hilo.
	// Routea las notifiaciones y mensajes de entrada
	virtual void run();
};

#endif
