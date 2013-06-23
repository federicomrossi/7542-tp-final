//
//  client_sincronizador.h
//  CLASE SINCRONIZADOR
//  


#ifndef SINCRONIZADOR_H
#define SINCRONIZADOR_H


#include <string>
#include "common_mutex.h"
#include "common_lock.h"
#include "common_lista.h"
#include "common_logger.h"
#include "client_emisor.h"

//DEBUG
#include <iostream>
//END DEBUG


/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Sincronizador {
private:

	Mutex m;									// Mutex
	Emisor *emisor;								// Emisor de mensajes
	Logger *logger;								// Logger de eventos

public:

	// Constructor
	Sincronizador(Emisor *emisor, Logger *logger);

	// Destructor
	~Sincronizador();

	// Crea el evento de envío de un archivo nuevo
	void enviarArchivo(std::string& nombreArchivo, std::string& contenido);

	// Crea el evento de modificar un archivo existente.
	// PRE: 'nombreArchivo' es el nombre de archivo que debe modificarse;
	// 'bloques' son pares de (bloque, contenido) los cuales son enviados para
	// ser actualizados en el servidor.
	void modificarArchivo(std::string& nombreArchivo, unsigned int bytesTotal, 
		Lista< std::pair< int, std::string > > bloques);

	// Crea el evento de eliminación de un archivo.
	// PRE: 'nombreArchivo' es el nombre de archivo que debe eliminarse.
	void eliminarArchivo(std::string& nombreArchivo);

	// Crea el evento de solicitud de un archivo nuevo.
	// PRE: 'nombreArchivo' esl el nombre del archivo.
	void solicitarArchivoNuevo(std::string& nombreArchivo);

	// Crea el evento de solicitud de modificación de un archivo.
	// PRE: 'nombreArchivo' esl el nombre del archivo; 'bloquesASolicitar' es
	// una lista de números de bloque a solicitar.
	void solicitarBloquesModificados(std::string& nombreArchivo, 
		Lista< int > bloquesASolicitar);
};

#endif
