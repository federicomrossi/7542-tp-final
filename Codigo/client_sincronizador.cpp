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
Sincronizador::Sincronizador(Emisor *emisor) : emisor(emisor) {
	// Armamos mensaje inicial solicitando lista de archivos del servidor.
	std::string mensaje = C_GET_FILES_LIST;

	// Enviamos mensaje al emisor
	this->emisor->ingresarMensajeDeSalida(mensaje);
}


// Destructor
Sincronizador::~Sincronizador() { }


// Crea el evento de envío de un archivo nuevo
void Sincronizador::enviarArchivo(std::string nombreArchivo, std::string contenido) {
	// Bloqueamos el mutex
	Lock l(m);

	// Armamos mensaje
	std::string mensaje = COMMON_SEND_FILE + " " + nombreArchivo + " " 
		+ contenido;

	// Enviamos mensaje al emisor
	this->emisor->ingresarMensajeDeSalida(mensaje);
}


// Crea el evento de modificar un archivo existente.
// PRE: 'nombreArchivo' es el nombre de archivo que debe modificarse;
// 'bloque' es el bloque del archivo que debe ser modificado; 'contenido'
// son los datos que deben reemplazarse por los existentes.
void Sincronizador::modificarArchivo(std::string nombreArchivo, int bloque, 
	std::string contenido) {
	// Bloqueamos el mutex
	Lock l(m);

	// Armamos mensaje
	std::string mensaje = COMMON_MODIFY_FILE + " " + nombreArchivo + " " 
		+ contenido;

	// Enviamos mensaje al emisor
	this->emisor->ingresarMensajeDeSalida(mensaje);
}


// Crea el evento de eliminación de un archivo.
// PRE: 'nombreArchivo' es el nombre de archivo que debe eliminarse.
void Sincronizador::eliminarArchivo(std::string nombreArchivo) {
	// Bloqueamos el mutex
	Lock l(m);

	// Armamos mensaje
	std::string mensaje = COMMON_DELETE_FILE + " " + nombreArchivo;

	// Enviamos mensaje al emisor
	this->emisor->ingresarMensajeDeSalida(mensaje);
}


// Crea el evento de solicitud de un archivo nuevo.
void Sincronizador::solicitarArchivoNuevo() {
	// Bloqueamos el mutex
	Lock l(m);
	//DEBUG
	std::cout << "Solicito archivo nuevo" << std::endl;
	//END DEBUG
}


// Crea el evento de solicitud de modificación de un archivo.
void Sincronizador::solicitarArchivoModificado() {
	// Bloqueamos el mutex
	Lock l(m);
	//DEBUG
	std::cout << "Solicito archivo modificado" << std::endl;
	//END DEBUG	
}

void Sincronizador::recibirNotificacion(std::string &notificacion) {
	if (notificacion.find(S_NOTIFY_NEW, 0) != std::string::npos)
		this->solicitarArchivoNuevo();
	else {
		if (notificacion.find(S_NOTIFY_CHANGE, 0) != std::string::npos)
			this->solicitarArchivoModificado();
	}
}
