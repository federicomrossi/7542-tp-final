//
//  client_sincronizador.h
//  CLASE SINCRONIZADOR
//  


#ifndef SINCRONIZADOR_H
#define SINCRONIZADOR_H


#include <string>
#include "common_mutex.h"
#include "common_lock.h"
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

public:

	// Constructor
	Sincronizador(Emisor *emisor);

	// Destructor
	~Sincronizador();

	// Crea el evento de envío de un archivo nuevo
	void enviarArchivo(std::string& nombreArchivo, std::string contenido);

	// Crea el evento de modificar un archivo existente.
	// PRE: 'nombreArchivo' es el nombre de archivo que debe modificarse;
	// 'bloque' es el bloque del archivo que debe ser modificado; 'contenido'
	// son los datos que deben reemplazarse por los existentes.
	void modificarArchivo(std::string& nombreArchivo, int bloque, 
		std::string contenido);

	// Crea el evento de eliminación de un archivo.
	// PRE: 'nombreArchivo' es el nombre de archivo que debe eliminarse.
	void eliminarArchivo(std::string& nombreArchivo);

	// Crea el evento de solicitud de un archivo nuevo.
	void solicitarArchivoNuevo(std::string& nombreArchivo);

	// Crea el evento de solicitud de modificación de un archivo.
	void solicitarArchivoModificado();
	
	// Recibe una notificacion y a partir del contenido
	// solicita archivo nuevo, modificado o elimina un archivo existente 
	// void recibirNotificacion(std::string& notificacion);
};

#endif
