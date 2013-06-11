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


		// Caso en que un cliente solicita un archivo
		if(instruccion == C_FILE_REQUEST) {
			Archivo a;
			std::string msg_salida;

			// Buscamos el archivo en el directorio y armamos mensaje
			if(!this->manejadorDeArchivos->obtenerArchivo(args, a)){
				msg_salida = S_NO_SUCH_FILE + " " + args;
				std::cout << "NO EXISTE ARCHIVO: " << msg_salida << std::endl;
			}
			else{
				msg_salida = COMMON_SEND_FILE + " " + a.obtenerNombre()
					+ " " + a.obtenerNumBloque() + " " + a.obtenerBloque() 
					+ " " + a.obtenerHash() + " " 
					+ a.obtenerFechaDeModificacion();
				std::cout << "EXISTE ARCHIVO: " << msg_salida << std::endl;
			}

			// Enviamos mensaje al cliente que realizó solicitud
			this->emisor->ingresarMensajeDeSalida(mensaje.first, msg_salida);
		}
		else if (instruccion == C_GET_FILES_LIST) {	
			// Se crea el mensaje de respuesta
			std::string respuesta = S_FILES_LIST + " ";
			
			// Pide la lista de archivos que tiene el server
			Lista<Archivo>* lista = new Lista<Archivo>;
			this->manejadorDeArchivos->obtenerArchivosDeDirectorio(lista);
			std::string lista_string;
			
			// Se guarda la lista en un string
			while (!lista->estaVacia()) {
				Archivo aux(lista->verPrimero());
				lista_string.append(aux.obtenerNombre());
				lista_string.append(" ");
				lista_string.append(aux.obtenerHash());
				lista_string.append(" ");
				lista_string.append(aux.obtenerFechaDeModificacion());
				lista_string.append(" ");
				lista->eliminarPrimero();
			}
			delete(lista);

			// Se agrega la lista al mensaje de respuesta
			respuesta.append(lista_string);

			// DEBUG
			std::cout << "Lista de archivos: " << lista_string << std::endl;
			//END DEBUG
			
			// Se envia la respuesta al cliente
			this->emisor->ingresarMensajeDeSalida(mensaje.first, respuesta);
		}
		else if (instruccion == COMMON_SEND_FILE) {

		}
		else if (instruccion == COMMON_MODIFY_FILE) {

		}
		else if (instruccion == COMMON_DELETE_FILE) {
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
