//  
//  server_sincronizador.h
//  CLASE SINCRONIZADOR
//  


#include <sstream> 
#include <utility>
#include "common_protocolo.h"
#include "common_parser.h"
#include "common_convertir.h"
#include "server_sincronizador.h"

// DEBUG
#include <iostream>
// END DEBUG




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Sincronizador::Sincronizador(Receptor *receptor, Emisor *emisor, 
	ManejadorDeArchivos *manejadorDeArchivos) : 
	receptor(receptor), emisor(emisor), 
	manejadorDeArchivos(manejadorDeArchivos) { }


// Destructor
Sincronizador::~Sincronizador() { }


// Inicia el sincronizador.
void Sincronizador::iniciar() {
	this->start();
}


// Detiene al sincronizador.
void Sincronizador::detener() {
	// Detenemos thread
	this->stop();

	// Insertamos mensaje fantasma para poder destrabar la recepción.
	this->receptor->ingresarMensajeDeEntrada(0, "");
}


// Define tareas a ejecutar en el hilo.
// Toma los mensajes que van llegando, los procesa y responde a clientes.
void Sincronizador::run() {
	// Procesamos mensajes entrantes
	while (this->isActive()) {
		// Solicitamos un mensaje de entrada al receptor
		std::pair < int, std::string > mensaje;
		mensaje = this->receptor->obtenerMensajeDeEntrada();

		if(!this->isActive()) break;

		// Tomamos instrucción y sus argumentos
		std::string instruccion, args;
		Parser::parserInstruccion(mensaje.second, instruccion, args);


		// Caso en que el cliente solicita la lista de archivos del servidor
		if (instruccion == C_GET_FILES_LIST) {	
			// Se crea el mensaje de respuesta
			std::string respuesta;
			respuesta.append(S_FILES_LIST);
			respuesta.append(" ");
			
			// Pide la lista de archivos que tiene el server
			Lista<std::string>* lista = new Lista<std::string>;
			this->manejadorDeArchivos->obtenerArchivosDeDirectorio(lista);
			
			// Insertamos como cabecera de los argumentos la cantidad de
			// archivos
			int cantArchivos = lista->tamanio();
			respuesta.append(Convertir::itos(cantArchivos));
			if(cantArchivos > 0) respuesta.append(COMMON_DELIMITER);
			
			// Se guarda la lista en un string
			while (!lista->estaVacia()) {
				std::string nombreArchivo = lista->verPrimero();
				std::string hashArchivo;
				
				int cantBloques = this->manejadorDeArchivos->obtenerHash(
					nombreArchivo, hashArchivo);
				
				respuesta.append(nombreArchivo);
				respuesta.append(COMMON_DELIMITER);
				respuesta.append(hashArchivo);
				respuesta.append(COMMON_DELIMITER);
				respuesta.append(Convertir::itos(cantBloques));
				
				// Eliminamos de la lista
				lista->eliminarPrimero();
				
				// Separamos del próximo archivo que se liste
				if(!lista->estaVacia()) 
					respuesta.append(COMMON_DELIMITER);
			}
			
			delete(lista);

			// DEBUG
			std::cout << "RESPUESTA GENERADA: " << respuesta << std::endl;
			//END DEBUG
			
			// Se envia la respuesta al cliente
			this->emisor->ingresarMensajeDeSalida(mensaje.first, respuesta);
		}
		// Caso en que un cliente solicita bloques de un archivo
		else if(instruccion == C_FILE_PARTS_REQUEST) {
			// Parseamos la lista de archivos enviada por el servidor
			Lista< std::string > listaArgumentos;
			Parser::dividirCadena(args, &listaArgumentos, COMMON_DELIMITER[0]);

			// Tomamos nombre de archivo
			std::string nombreArchivo = listaArgumentos.verPrimero();
			listaArgumentos.eliminarPrimero();

			std::string respuesta;

			// Caso en que no existe el archivo en el servidor
			if(!this->manejadorDeArchivos->existeArchivo(nombreArchivo)) {
				respuesta.append(S_NO_SUCH_FILE);
				respuesta.append(" ");
				respuesta.append(nombreArchivo);
			}
			// Caso en que existe el archivo en el servidor
			else {
				// Armamos el mensaje de respuesta
				respuesta.append(COMMON_FILE_PARTS);
				respuesta.append(" ");
				respuesta.append(nombreArchivo);
				respuesta.append(COMMON_DELIMITER);
				respuesta.append(Convertir::uitos(
					this->manejadorDeArchivos->obtenerCantBytes(
						nombreArchivo)));

				// Iteramos sobre los bloques solicitados
				while(!listaArgumentos.estaVacia()) {
					// Tomamos un número de bloque
					std::string sNumBloque = listaArgumentos.verPrimero();
					listaArgumentos.eliminarPrimero();
					int numBloque = Convertir::stoi(sNumBloque);

					// Insertamos número de bloque y su contenido
					respuesta.append(COMMON_DELIMITER);
					respuesta.append(sNumBloque);
					respuesta.append(COMMON_DELIMITER);
					respuesta.append(
						this->manejadorDeArchivos->obtenerContenido(
						nombreArchivo, numBloque));
				}
			}

			// DEBUG
			std::cout << "RESPUESTA GENERADA: " << respuesta << std::endl;
			//END DEBUG

			// Se envia la respuesta al cliente
			this->emisor->ingresarMensajeDeSalida(mensaje.first, respuesta);
		}
		// Caso en que un cliente solicita un archivo
		else if(instruccion == C_FILE_REQUEST) {
			// Armamos mensaje de respuesta
			std::string respuesta;

			// Caso en que no existe el archivo en el servidor
			if(!this->manejadorDeArchivos->existeArchivo(args)) {
				respuesta.append(S_NO_SUCH_FILE);
				respuesta.append(" ");
				respuesta.append(args);
			}
			// Caso en que existe el archivo en el servidor
			else {
				// Armamos mensaje con contenido del archivo
				respuesta.append(COMMON_SEND_FILE);
				respuesta.append(" ");
				respuesta.append(args);
				respuesta.append(COMMON_DELIMITER);
				respuesta.append(this->manejadorDeArchivos->obtenerContenido(
					args, 0));
			}

			// Enviamos mensaje al cliente que realizó solicitud
			this->emisor->ingresarMensajeDeSalida(mensaje.first, respuesta);
		}
		else if (instruccion == COMMON_SEND_FILE) {
			// Parseamos la lista de archivos enviada por el servidor
			Lista< std::string > listaArgumentos;
			Parser::dividirCadena(args, &listaArgumentos, COMMON_DELIMITER[0]);

			// Agregamos el archivo en el servidor
			this->manejadorDeArchivos->agregarArchivo(listaArgumentos[0],
				listaArgumentos[1]);

			// Enviamos notificación a clientes de que se agregó archivo
			std::string msg_salida;
			msg_salida.append(S_NEW_FILE);
			msg_salida.append(" ");
			msg_salida.append(listaArgumentos[0]);

			// Se envia la notificación de nuevo archivo a los clientes
			this->emisor->ingresarMensajeDeSalida(0, msg_salida);
		}
		// Caso en que se recibe la notificación de la modificación de archivo
		else if (instruccion == C_MODIFY_FILE) {

		}
		// Caso en que se recibe la notificación de la eliminación de archivo
		else if (instruccion == COMMON_DELETE_FILE) {
			// Parseamos la lista de archivos enviada por el servidor
			Lista< std::string > listaArgumentos;
			Parser::dividirCadena(args, &listaArgumentos, COMMON_DELIMITER[0]);

			// Eliminamos archivo en carpeta del servidor
			this->manejadorDeArchivos->eliminarArchivo(listaArgumentos[0]);

			// Enviamos notificación a clientes de que se eliminó archivo
			std::string msg_salida;
			msg_salida.append(COMMON_DELETE_FILE);
			msg_salida.append(" ");
			msg_salida.append(listaArgumentos[0]);

			// Se envia la notificación de nuevo archivo a los clientes
			this->emisor->ingresarMensajeDeSalida(0, msg_salida);
		}
	}
}
