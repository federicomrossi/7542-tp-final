//  
//  server_sincronizador.h
//  CLASE SINCRONIZADOR
//  


#include <sstream> 
#include <utility>
#include "common_protocolo.h"
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
		parserMensaje(mensaje.second, instruccion, args);


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





/*
 * IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */


// Parsea el mensaje separando la instruccion de sus argumentos.
// PRE: 'msg' es el mensaje que desea parsearse; 'instruccion' y 'args' son
// referencias a variables en donde se desea almacenar la instruccion y sus
// argumentos respectivamente.
void Sincronizador::parserMensaje(const std::string& msg, 
	std::string& instruccion, std::string& args) {
	std::stringstream msgTemp(msg);

	// Tomamos la instrucción
	msgTemp >> instruccion;
	getline(msgTemp, args);

	// Eliminamos el espacio inicial sobrante de los argumentos
	if(args != "") args.erase(0, 1);
}


// Implementar parser generico de argumentos







// BORRAR PARSERS DE ACA PARA ABAJO!!!!
//////////////////////////////////////////////////////////////////


// // Parsea los datos de un archivo
// // PRE: 'args' es la cadena que contiene los datos separados por una coma: 
// // [NOMBRE],[NUM_BLOQUE],[BLOQUE],[HASH],[FECHA]; 'archivo' es un puntero 
// // al objeto Archivo en donde se almacenarán dichos datos.
// void Sincronizador::parserArchivo(const std::string argumentos, Archivo *archivo) {

// 	// El mensaje viene en el formato "<Nombre,Numero_Bloque,Bloque,Hash,Fecha>"
// 	// Divididos por una ','
// 	std::string args[5];
// 	std::string aux;
// 	std::string msj = argumentos;
// 	int i;
// 	int delim = 0;
	
// 	// Se parsea el mensaje
// 	for (i = 0; i < 5; i++) {
// 		delim = msj.find(COMMON_DELIMITER);
// 		aux = msj.substr(0, delim);
// 		msj.erase(0, delim + 1);
// 		args[i].assign(aux.c_str());
// 	}	

// 	archivo->asignarNombre(args[0]);
// 	archivo->asignarNumBloque(args[1]);
// 	archivo->asignarBloque(args[2]);
// 	archivo->asignarHash(args[3]);
// 	archivo->asignarFechaDeModificacion(args[4]);
// }
