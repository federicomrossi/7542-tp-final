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
	Inspector *inspector, ReceptorDeArchivos *receptorDeArchivos) :
	receptor(receptor), inspector(inspector), 
	receptorDeArchivos(receptorDeArchivos) { }


// Destructor
ManejadorDeNotificaciones::~ManejadorDeNotificaciones() { }


// Define tareas a ejecutar en el hilo.
// Routea las notifiaciones y mensajes de entrada
void ManejadorDeNotificaciones::run() {
	std::string mensaje;
	while (this->isActive()) {
		// Pide un mensaje de la cola al receptor
		mensaje = this->receptor->obtenerMensajeDeEntrada();

		// Formato de notificacion: "<'n'><Instruccion,Arg1-Arg2-Arg3..>"
		if (mensaje[0] == 'n')  { // Es una notificacion
			mensaje.erase(0,1);  // Se borra el identificador

			//DEBUG
			std::cout << "Es notificacion" << std::endl;
			//FIN DEBUG
	
			// Se envia la notificacion al sincronizador para que 
			// realice una accion en base a ella.
			// this->sincronizador->recibirNotificacion(mensaje);
		}
		// Formato de archivo: "<'f'><Instruccion,Arg1-Arg2-Arg3..>"
		else {
			if (mensaje[0] == 'f') {  // Es un archivo
				mensaje.erase(0,1);  // Se borra el identificador

				//DEBUG
				std::cout << "Es archivo" << std::endl;
				//FIN DEBUG
				
				// Se envia el archivo con la accion a realizar sobre el 
				// al receptor de archivos
				this->receptorDeArchivos->recibirArchivo(mensaje);
			}
			else  // Con cualquier otro mensaje, frena
				this->stop();  
		}
		mensaje.clear();
	}
}

