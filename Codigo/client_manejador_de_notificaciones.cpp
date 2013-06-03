//
//  client_manejador_de_notificaciones.cpp
//  CLASE MANEJADORDENOTIFICACIONES
//  


#include "client_manejador_de_notificaciones.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
ManejadorDeNotificaciones::ManejadorDeNotificaciones(Receptor *receptor,
	Sincronizador *sincronizador, ReceptorDeArchivos *receptorDeArchivos) :
	receptor(receptor), sincronizador(sincronizador), 
	receptorDeArchivos(receptorDeArchivos) { }


// Destructor
ManejadorDeNotificaciones::~ManejadorDeNotificaciones() { }


// Define tareas a ejecutar en el hilo.
// Routea las notifiaciones y mensajes de entrada
void ManejadorDeNotificaciones::run() {
	
}
