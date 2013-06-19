//
//  client_manejador_de_notificaciones.cpp
//  CLASE MANEJADORDENOTIFICACIONES
//  


#include <sstream>
#include "common_parser.h" 
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
			// Derivamos al receptor de archivos
			this->receptorDeArchivos->recibirArchivo(mensaje);
		}
		else if(instruccion == COMMON_DELETE_FILE) {
			// Derivamos al receptor de archivos
			std::cout << "NOTIFICACION DE QUE SE BORRO ARCHIVO" << std::endl;
			
			// Derivamos al receptor de archivos
			this->receptorDeArchivos->eliminarArchivo(args);
		}


		// // Formato de notificacion: "<'n'><Instruccion,Arg1-Arg2-Arg3..>"
		// if (mensaje[0] == 'n')  { // Es una notificacion
		// 	mensaje.erase(0,1);  // Se borra el identificador

		// 	//DEBUG
		// 	std::cout << "Es notificacion" << std::endl;
		// 	//FIN DEBUG
	
		// 	// Se envia la notificacion al sincronizador para que 
		// 	// realice una accion en base a ella.
		// 	// this->sincronizador->recibirNotificacion(mensaje);
		// }
		// // Formato de archivo: "<'f'><Instruccion,Arg1-Arg2-Arg3..>"
		// else {
		// 	if (mensaje[0] == 'f') {  // Es un archivo
		// 		mensaje.erase(0,1);  // Se borra el identificador

		// 		//DEBUG
		// 		std::cout << "Es archivo" << std::endl;
		// 		//FIN DEBUG
				
		// 		// Se envia el archivo con la accion a realizar sobre el 
		// 		// al receptor de archivos
		// 		this->receptorDeArchivos->recibirArchivo(mensaje);
		// 	}
		// 	else  // Con cualquier otro mensaje, frena
		// 		this->stop();  
		// }
		// mensaje.clear();
	}
}




/*
 * IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */


// Parsea el mensaje separando la instruccion de sus argumentos.
// PRE: 'msg' es el mensaje que desea parsearse; 'instruccion' y 'args' son
// referencias a variables en donde se desea almacenar la instruccion y sus
// argumentos respectivamente.
void ManejadorDeNotificaciones::parserMensaje(const std::string& msg, 
	std::string& instruccion, std::string& args) {
	std::stringstream msgTemp(msg);

	// Tomamos la instrucción
	msgTemp >> instruccion;
	getline(msgTemp, args);

	// Eliminamos el espacio inicial sobrante de los argumentos
	if(args != "") args.erase(0, 1);
}
