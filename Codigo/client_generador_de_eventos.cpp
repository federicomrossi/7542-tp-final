//
//  client_generador_de_eventos.cpp
//  CLASE GENERADORDEEVENTOS
//  


#include "client_generador_de_eventos.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
GeneradorDeEventos::GeneradorDeEventos() { }


// Destructor
GeneradorDeEventos::~GeneradorDeEventos() { }


// Crea el evento de envío de un archivo nuevo
void GeneradorDeEventos::enviarArchivo(std::string nombreArchivo, std::string contenido) {

}


// Crea el evento de modificar un archivo existente.
// PRE: 'nombreArchivo' es el nombre de archivo que debe modificarse;
// 'bloque' es el bloque del archivo que debe ser modificado; 'contenido'
// son los datos que deben reemplazarse por los existentes.
void GeneradorDeEventos::modificarArchivo(std::string nombreArchivo, int bloque, 
	std::string contenido) {

}


// Crea el evento de eliminación de un archivo.
// PRE: 'nombreArchivo' es el nombre de archivo que debe eliminarse.
void GeneradorDeEventos::eliminarArchivo(std::string nombreArchivo) {

}


// Crea el evento de solicitud de un archivo nuevo.
void GeneradorDeEventos::solicitarArchivoNuevo() {

}


// Crea el evento de solicitud de modificación de un archivo.
void GeneradorDeEventos::solicitarArchivoModificado() {

}
