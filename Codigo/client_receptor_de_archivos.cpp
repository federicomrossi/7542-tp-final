//
//  client_receptor_de_archivos.cpp
//  CLASE RECEPTORDEARCHIVOS
//  



#include "common_mutex.h"
#include "common_lock.h"
#include "client_receptor_de_archivos.h"


/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
ReceptorDeArchivos::ReceptorDeArchivos(ManejadorDeArchivos *unManejador) :
	manejadorDeArchivos(unManejador) { }


// Destructor
ReceptorDeArchivos::~ReceptorDeArchivos() { }


// Se encarga de procesar la recepción de un archivo nuevo.
// PRE: 'nombreArchivo' es el nombre del archivo a recibir; 'contenido' es
// el contenido del archivo a recibir.
void ReceptorDeArchivos::recibirArchivo(const std::string& nombreArchivo,
	const std::string& contenido) {
	// Damos la orden de agregar el archivo
	this->manejadorDeArchivos->agregarArchivo(nombreArchivo, contenido);

	// Actualizamos el registro local de archivos
	this->manejadorDeArchivos->actualizarRegistroDeArchivos();
}


// Se encarga de procesar la eliminación de un archivo
void ReceptorDeArchivos::eliminarArchivo(std::string& nombreArchivo) {
	// Damos la orden de eliminar el archivo
	this->manejadorDeArchivos->eliminarArchivo(nombreArchivo);

	// Eliminamos el archivo del registro local
	this->manejadorDeArchivos->borrarDeRegistroDeArchivos(nombreArchivo);
}
