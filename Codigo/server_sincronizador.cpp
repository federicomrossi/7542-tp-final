//  
//  server_sincronizador.h
//  CLASE SINCRONIZADOR
//  


#include <sstream> 
#include <utility>
#include "common_protocolo.h"
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


// Define tareas a ejecutar en el hilo.
// Toma los mensajes que van llegando, los procesa y responde a clientes.
void Sincronizador::run() {
	
	while (this->isActive()) {
		// Solicitamos un mensaje de entrada al receptor
		std::pair < int, std::string > mensaje;
		mensaje = this->receptor->obtenerMensajeDeEntrada();

		// Tomamos instrucción y sus argumentos
		std::string instruccion, args;
		parserMensaje(mensaje.second, instruccion, args);


		// Caso en que un cliente solicita un archivo
		if(instruccion == C_FILE_REQUEST) {
			// Armamos mensaje y lo enviamos al cliente que realizó solicitud
			std::string msg_salida = COMMON_SEND_FILE + " ";
			this->emisor->ingresarMensajeDeSalida(mensaje.first, msg_salida);
		}
		else if(instruccion == COMMON_SEND_FILE) {
			
		}
		else if(instruccion == COMMON_MODIFY_FILE) {

		}
		else if(instruccion == COMMON_DELETE_FILE) {
			// Eliminamos archivo en carpeta del servidor
			std::string archivo = args;
			std::string num_bloque = "0";
			this->manejadorDeArchivos->eliminarArchivo(archivo, num_bloque);

			// Enviamos notificación a clientes de que se eliminó archivo
			std::string msg_salida = COMMON_DELETE_FILE + " " + args;
			this->emisor->ingresarMensajeDeSalida(0, msg_salida);
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