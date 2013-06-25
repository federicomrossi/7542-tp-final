//
//  client_sincronizador.cpp
//  CLASE SINCRONIZADOR
//  


#include <string>
#include "common_protocolo.h"
#include "common_convertir.h"
#include "client_sincronizador.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Sincronizador::Sincronizador(Emisor *emisor, Logger *logger) : emisor(emisor),
	logger(logger) { }


// Destructor
Sincronizador::~Sincronizador() { }


// Crea el evento de envío de un archivo nuevo
void Sincronizador::enviarArchivo(std::string& nombreArchivo, std::string& contenido, std::string hash) {
	// Bloqueamos el mutex
	Lock l(m);

	// Armamos mensaje
	std::string mensaje;
	mensaje.append(COMMON_SEND_FILE);
	mensaje.append(" ");
	mensaje.append(nombreArchivo);
	mensaje.append(COMMON_DELIMITER);
	mensaje.append(contenido);
	mensaje.append(COMMON_DELIMITER);
	mensaje.append(hash);


	// Mensaje de log
	this->logger->emitirLog("Se envió archivo '" + nombreArchivo + "' " +
		"al servidor.");

	// Enviamos mensaje al emisor
	this->emisor->ingresarMensajeDeSalida(mensaje);
}


// Crea el evento de modificar un archivo existente.
// PRE: 'nombreArchivo' es el nombre de archivo que debe modificarse;
// 'bloques' son pares de (bloque, contenido) los cuales son enviados para
// ser actualizados en el servidor.
void Sincronizador::modificarArchivo(std::string& nombreArchivo, 
	unsigned int bytesTotal, Lista< std::pair< int, std::string > > bloques) {
	// Bloqueamos el mutex
	Lock l(m);

	// Armamos mensaje
	std::string mensaje;
	mensaje.append(C_MODIFY_FILE);
	mensaje.append(" ");
	mensaje.append(nombreArchivo);
	mensaje.append(COMMON_DELIMITER);
	mensaje.append(Convertir::uitos(bytesTotal));
	
	// Iteramos sobre los bloques modificados. Si solo se cambio el tamaño
	// del archivo por el truncamiento de parte de su contenido final, solo
	// se envía la cantidad tota de bytes.
	while(!bloques.estaVacia()) {
		std::pair< int, std::string > bloque = bloques.verPrimero();
		bloques.eliminarPrimero();

		mensaje.append(COMMON_DELIMITER);
		mensaje.append(Convertir::uitos(bloque.first));
		mensaje.append(COMMON_DELIMITER);
		mensaje.append(bloque.second);
	}

	// Mensaje de log
	std::string log = "Se enviaron modificaciones hechas sobre el archivo '";
	log += nombreArchivo + "' al servidor.";
	this->logger->emitirLog(log);

	// Enviamos mensaje al emisor
	this->emisor->ingresarMensajeDeSalida(mensaje);
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

	// Mensaje de log
	this->logger->emitirLog("Se envió notificación de eliminar archivo '" +
		nombreArchivo + "' al servidor");

	// Enviamos mensaje al emisor
	this->emisor->ingresarMensajeDeSalida(mensaje);
}


// Crea el evento de solicitud de un archivo nuevo.
// PRE: 'nombreArchivo' esl el nombre del archivo.
void Sincronizador::solicitarArchivoNuevo(std::string& nombreArchivo) {
	// Bloqueamos el mutex
	Lock l(m);

	// Armamos mensaje
	std::string mensaje;
	mensaje.append(C_FILE_REQUEST);
	mensaje.append(" ");
	mensaje.append(nombreArchivo);

	// Mensaje de log
	this->logger->emitirLog("Se solicitó archivo '" + nombreArchivo + "' " +
		"al servidor.");

	// Enviamos mensaje al emisor
	this->emisor->ingresarMensajeDeSalida(mensaje);
}


// Crea el evento de solicitud de modificación de un archivo.
// PRE: 'nombreArchivo' esl el nombre del archivo; 'bloquesASolicitar' es
// una lista de números de bloque a solicitar.
void Sincronizador::solicitarBloquesModificados(std::string& nombreArchivo, 
	Lista< int > bloquesASolicitar) {
	// Bloqueamos el mutex
	Lock l(m);

	// Armamos mensaje
	std::string mensaje;
	mensaje.append(C_FILE_PARTS_REQUEST);
	mensaje.append(" ");
	mensaje.append(nombreArchivo);

	// Insertamos numeros de bloque en mensaje
	for(size_t i = 0; i < bloquesASolicitar.tamanio(); i++) {
		mensaje.append(COMMON_DELIMITER);
		mensaje.append(Convertir::itos(bloquesASolicitar[i]));
	}

	// Mensaje de log
	this->logger->emitirLog("Se solicitaron partes del archivo '" +
		nombreArchivo + "' al servidor");

	// Enviamos mensaje al emisor
	this->emisor->ingresarMensajeDeSalida(mensaje);
}
