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

			// Armamos el mensaje de respuesta
			std::string respuesta;
			respuesta.append(COMMON_FILE_PARTS);
			respuesta.append(" ");
			respuesta.append(nombreArchivo);

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
				respuesta.append(this->manejadorDeArchivos->obtenerContenido(
					nombreArchivo, numBloque));
			}

			// Se envia la respuesta al cliente
			this->emisor->ingresarMensajeDeSalida(mensaje.first, respuesta);
		}
		// Caso en que un cliente solicita un archivo
		else if(instruccion == C_FILE_REQUEST) {
			// Archivo a;
			// std::string msg_salida;

			// // Buscamos el archivo en el directorio y armamos mensaje
			// if(!this->manejadorDeArchivos->obtenerArchivo(args, a)){
			// 	// Armamos mensaje
			// 	msg_salida.append(S_NO_SUCH_FILE);
			// 	msg_salida.append(" ");
			// 	msg_salida.append(args);
			// 	std::cout << "NO EXISTE ARCHIVO: " << msg_salida << std::endl;
			// }
			// else{
			// 	// Armamos mensaje
			// 	msg_salida.append(COMMON_SEND_FILE);
			// 	msg_salida.append(" ");
			// 	msg_salida.append(a.obtenerNombre());
			// 	msg_salida.append(COMMON_DELIMITER);
			// 	msg_salida.append(a.obtenerNumBloque());
			// 	msg_salida.append(COMMON_DELIMITER);
			// 	msg_salida.append(a.obtenerBloque());
			// 	msg_salida.append(COMMON_DELIMITER);
			// 	msg_salida.append(a.obtenerHash());
			// 	msg_salida.append(COMMON_DELIMITER);
			// 	msg_salida.append(a.obtenerFechaDeModificacion());

			// 	std::cout << "EXISTE ARCHIVO: " << msg_salida << std::endl;
			// }

			// // Enviamos mensaje al cliente que realizó solicitud
			// this->emisor->ingresarMensajeDeSalida(mensaje.first, msg_salida);
		}
		else if (instruccion == COMMON_SEND_FILE) {
			// Archivo a;
			// this->parserArchivo(args, &a);

			// // Agregamos el archivo en el servidor
			// std::string nombre = a.obtenerNombre();
			// std::string bloque = a.obtenerBloque();
			// std::string hash = a.obtenerHash();
			// this->manejadorDeArchivos->agregarArchivo(nombre, WHOLE_FILE, 
			// 	bloque, hash);

			// // Enviamos notificación a clientes de que se agregó archivo
			// std::string msg_salida;
			// msg_salida.append(S_NOTIFY_NEW);
			// msg_salida.append(" ");
			// msg_salida.append(a.obtenerNombre());
			// this->emisor->ingresarMensajeDeSalida(0, msg_salida);
		}
		else if (instruccion == C_MODIFY_FILE) {

		}
		else if (instruccion == COMMON_DELETE_FILE) {
			// // Eliminamos archivo en carpeta del servidor
			// this->manejadorDeArchivos->eliminarArchivo(args, WHOLE_FILE);

			// // Enviamos notificación a clientes de que se eliminó archivo
			// std::string msg_salida;
			// msg_salida.append(COMMON_DELETE_FILE);
			// msg_salida.append(" ");
			// msg_salida.append(args);
			// this->emisor->ingresarMensajeDeSalida(0, msg_salida);
		}
	}
}
