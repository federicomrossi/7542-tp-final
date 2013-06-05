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

void ReceptorDeArchivos::recibirArchivo(std::string &archivo) {
	// Debe enviar al manejador de archivos el archivo recibido
	// Llega el archivo con 'ADD' o 'DEL' al comienzo
	// para saber si hay que agregar o eliminar un archivo del directorio
}
