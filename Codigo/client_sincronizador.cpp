//
//  client_sincronizador.cpp
//  CLASE SINCRONIZADOR
//  


#include "common_mutex.h"
#include "common_lock.h"
#include "client_sincronizador.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Sincronizador::Sincronizador() { }


// Destructor
Sincronizador::~Sincronizador() { }


// Crea el evento de envío de un archivo nuevo
void Sincronizador::enviarArchivo(std::string nombreArchivo, std::string contenido) {

}


// Crea el evento de modificar un archivo existente.
// PRE: 'nombreArchivo' es el nombre de archivo que debe modificarse;
// 'bloque' es el bloque del archivo que debe ser modificado; 'contenido'
// son los datos que deben reemplazarse por los existentes.
void Sincronizador::modificarArchivo(std::string nombreArchivo, int bloque, 
	std::string contenido) {

}


// Crea el evento de eliminación de un archivo.
// PRE: 'nombreArchivo' es el nombre de archivo que debe eliminarse.
void Sincronizador::eliminarArchivo(std::string nombreArchivo) {

}


// Crea el evento de solicitud de un archivo nuevo.
void Sincronizador::solicitarArchivoNuevo() {

}


// Crea el evento de solicitud de modificación de un archivo.
void Sincronizador::solicitarArchivoModificado() {

}
