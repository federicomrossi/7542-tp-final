//
//  client_manejador_de_notificaciones.cpp
//  CLASE MANEJADORDENOTIFICACIONES
//  


#include <sstream>
#include "common_parser.h"
#include "common_lista.h"
#include "common_protocolo.h"
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
	// Procesamos mensajes entrantes
	while (this->isActive()) {
		// Pide un mensaje de la cola al receptor
		std::string mensaje;
		mensaje = this->receptor->obtenerMensajeDeEntrada();

		if(!this->isActive()) break;

		// Tomamos instrucción y sus argumentos
		std::string instruccion, args;
		Parser::parserInstruccion(mensaje, instruccion, args);


		// Caso en que se notifica la existencia de un nuevo archivo
		if(instruccion == S_NEW_FILE) {
			// Derivamos al inspector
			this->inspector->inspeccionarExisteArchivo(args);
		}
		else if(instruccion == S_FILE_CHANGED) {
			// Derivamos al inspector
		}
		else if(instruccion == COMMON_SEND_FILE) {
			// Parseamos argumentos para obtener nombre y contenido del archivo
			Lista< std::string > listaArgumentos;
			Parser::dividirCadena(args, &listaArgumentos, COMMON_DELIMITER[0]);

			// Derivamos al receptor de archivos
			this->receptorDeArchivos->recibirArchivo(listaArgumentos[0],
				listaArgumentos[1]);
		}
		else if(instruccion == COMMON_DELETE_FILE) {
			// DEBUG
			std::cout << "NOTIFICACION DE QUE SE BORRO ARCHIVO" << std::endl;
			// END DEBUG

			// Derivamos al receptor de archivos
			this->receptorDeArchivos->eliminarArchivo(args);
		}

	}
}
