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
	
	// Formato de archivo: "<Accion><Nombre_Archivo-Archivo>"
	// Donde 'Accion' es 'ADD' o 'DEL'

	int delim = archivo.find('-');
	std::string nombre_archivo = archivo.substr(3, delim - 3);
	archivo = archivo.substr(delim + 1, std::string::npos);
	if (archivo.find("ADD"))
		this->manejadorDeArchivos->agregarArchivo(nombre_archivo, archivo);
	else {
		if (archivo.find("DEL"))
			this->manejadorDeArchivos->eliminarArchivo(nombre_archivo);
	}
}
