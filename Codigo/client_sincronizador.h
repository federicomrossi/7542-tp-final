//
//  client_sincronizador.h
//  CLASE SINCRONIZADOR
//  


#ifndef SINCRONIZADOR_H
#define SINCRONIZADOR_H


#include <string>
#include "client_manejador_de_archivos.h"
class Mutex;
class Lock;





/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Sincronizador {
private:

	Mutex m;										// Mutex
	ManejadorDeArchivos *manejadorDeArchivos;		// Manejador de archivos

public:

	// Constructor
	Sincronizador(ManejadorDeArchivos *unManejador);

	// Destructor
	~Sincronizador();

	// Crea el evento de envío de un archivo nuevo
	void enviarArchivo(std::string nombreArchivo, std::string contenido);

	// Crea el evento de modificar un archivo existente.
	// PRE: 'nombreArchivo' es el nombre de archivo que debe modificarse;
	// 'bloque' es el bloque del archivo que debe ser modificado; 'contenido'
	// son los datos que deben reemplazarse por los existentes.
	void modificarArchivo(std::string nombreArchivo, int bloque, 
		std::string contenido);

	// Crea el evento de eliminación de un archivo.
	// PRE: 'nombreArchivo' es el nombre de archivo que debe eliminarse.
	void eliminarArchivo(std::string nombreArchivo);

	// Crea el evento de solicitud de un archivo nuevo.
	void solicitarArchivoNuevo();

	// Crea el evento de solicitud de modificación de un archivo.
	void solicitarArchivoModificado();
};

#endif
