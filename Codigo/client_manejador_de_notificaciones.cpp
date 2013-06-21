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
			// Parseamos argumentos
			Lista< std::string > listaArgumentos;
			Parser::dividirCadena(args, &listaArgumentos, COMMON_DELIMITER[0]);
			
			// Tomamos nombre de archivo
			std::string nombreArchivo = listaArgumentos.verPrimero();
			listaArgumentos.eliminarPrimero();
			
			// Tomamos cantidad de bytes que debe tener el archivo ahora
			std::string sCantBytesTotal = listaArgumentos.verPrimero();
			unsigned int cantBytesTotal = Convertir::stoui(sCantBytesTotal);
			listaArgumentos.eliminarPrimero();
			
			// Lista de bloques a inspeccionar
			Lista< std::pair< int, std::string > > bloques;

			// Tomamos los bloques y sus hashes
			while(!listaArgumentos.estaVacia()) {
				int bloque = Convertir::stoui(listaArgumentos.verPrimero());
				listaArgumentos.eliminarPrimero();
				std::string hash = listaArgumentos.verPrimero();
				listaArgumentos.eliminarPrimero();

				bloques.insertarUltimo(std::make_pair(bloque, hash));
			}
			
			// Derivamos al inspector
			this->inspector->inspeccionarArchivo(nombreArchivo, 
				cantBytesTotal, bloques);
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
		else if(instruccion == COMMON_FILE_PARTS) {
			// Parseamos los argumentos de la respuesta
			Lista< std::string > listaArgumentos;
			Parser::dividirCadena(args, &listaArgumentos, COMMON_DELIMITER[0]);

			// Tomamos el nombre de archivo
			std::string archivoEntrante = listaArgumentos.verPrimero();
			listaArgumentos.eliminarPrimero();

			// Tomamos la cantidad total de bytes del archivo
			unsigned int cantTotalBytes;
			cantTotalBytes = Convertir::stoui(listaArgumentos.verPrimero());
			listaArgumentos.eliminarPrimero();

			// Creamos lista de bloques a reemplazar
			Lista< std::pair< int, std::string > > listaBloquesAReemplazar;

			// LLenamos la lista de bloques a reemplazar
			while(!listaArgumentos.estaVacia()) {
				// Tomamos un número de bloque
				int numBloque = Convertir::stoi(listaArgumentos.verPrimero());
				listaArgumentos.eliminarPrimero();
				
				// Tomamos el contenido del bloque
				std::string contenidoBloque = listaArgumentos.verPrimero();
				listaArgumentos.eliminarPrimero();

				// Insertamos bloque en lista de bloques
				listaBloquesAReemplazar.insertarUltimo(std::make_pair(
					numBloque, contenidoBloque));
			}

			// Derivamos al receptor de archivos
			this->receptorDeArchivos->recibirModificaciones(archivoEntrante,
				cantTotalBytes, listaBloquesAReemplazar);
		}
		else if(instruccion == S_NO_SUCH_FILE) {
			// DEBUG
			std::cout << args << ": No existe dicho archivo solcitado." << std::endl;
			// END DEBUG
		}
	}
}
