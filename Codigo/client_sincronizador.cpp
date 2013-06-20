//
//  client_sincronizador.cpp
//  CLASE SINCRONIZADOR
//  


#include <string>
#include "common_protocolo.h"
#include "client_sincronizador.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Sincronizador::Sincronizador(Emisor *emisor) : emisor(emisor) { }


// Destructor
Sincronizador::~Sincronizador() { }


// Crea el evento de envío de un archivo nuevo
void Sincronizador::enviarArchivo(std::string& nombreArchivo, std::string& contenido) {
	// Bloqueamos el mutex
	Lock l(m);

	// Armamos mensaje
	std::string mensaje;
	mensaje.append(COMMON_SEND_FILE);
	mensaje.append(" ");
	mensaje.append(nombreArchivo);
	mensaje.append(COMMON_DELIMITER);
	mensaje.append(contenido);

	// Enviamos mensaje al emisor
	this->emisor->ingresarMensajeDeSalida(mensaje);
}


// Crea el evento de modificar un archivo existente.
// PRE: 'nombreArchivo' es el nombre de archivo que debe modificarse;
// 'bloques' son pares de (bloque, contenido) los cuales son enviados para
// ser actualizados en el servidor.
void Sincronizador::modificarArchivo(std::string& nombreArchivo, 
		Lista< std::pair< int, std::string > > bloques) {
	// Bloqueamos el mutex
	Lock l(m);

	// // Armamos mensaje
	// std::string mensaje = C_MODIFY_FILE + " " + nombreArchivo + " " 
	// 	+ contenido;

	// // Enviamos mensaje al emisor
	// this->emisor->ingresarMensajeDeSalida(mensaje);
}


// Crea el evento de eliminación de un archivo.
// PRE: 'nombreArchivo' es el nombre de archivo que debe eliminarse.
void Sincronizador::eliminarArchivo(std::string& nombreArchivo) {
	// Bloqueamos el mutex
	Lock l(m);

	// Armamos mensaje
	std::string mensaje;
	mensaje.append(COMMON_DELETE_FILE);
	mensaje.append(" ");
	mensaje.append(nombreArchivo);

	// Enviamos mensaje al emisor
	this->emisor->ingresarMensajeDeSalida(mensaje);
}


// Crea el evento de solicitud de un archivo nuevo.
void Sincronizador::solicitarArchivoNuevo(std::string& nombreArchivo) {
	// Bloqueamos el mutex
	Lock l(m);

	// Armamos mensaje
	std::string mensaje;
	mensaje.append(C_FILE_REQUEST);
	mensaje.append(" ");
	mensaje.append(nombreArchivo);

	// Enviamos mensaje al emisor
	this->emisor->ingresarMensajeDeSalida(mensaje);

	//DEBUG
	std::cout << "Solicito archivo nuevo" << std::endl;
	//END DEBUG
}


// Crea el evento de solicitud de modificación de un archivo.
void Sincronizador::solicitarBloquesModificados() {
	// Bloqueamos el mutex
	Lock l(m);

	//DEBUG
	std::cout << "Solicito archivo modificado" << std::endl;
	//END DEBUG	
}


// void Sincronizador::recibirNotificacion(std::string &notificacion) {
// 	if (notificacion.find(S_NOTIFY_NEW, 0) != std::string::npos)
// 		this->solicitarArchivoNuevo();
// 	else {
// 		if (notificacion.find(S_NOTIFY_CHANGE, 0) != std::string::npos)
// 			this->solicitarArchivoModificado();
// 	}
// }
