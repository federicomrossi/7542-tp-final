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


// Debe enviar al manejador de archivos el archivo recibido en el mensaje
// Llega el archivo con COMMON_SEND_FILE o COMMON_DELETE_FILE al comienzo
// para saber si hay que agregar un bloque
// o eliminar un archivo del directorio

// Formato de archivo: "<Instruccion,Nombre_Archivo,Numero_Bloque,Bloque_Archivo>".
// Para hacer referencia a todo el archivo, Numero_Bloque = WHOLE_FILE
void ReceptorDeArchivos::recibirArchivo(std::string &mensaje) {
	
	std::string accion;
	std::string nombre_archivo;
	std::string num_bloque;
	std::string bloque_archivo;
	parsearMensaje(mensaje, accion, nombre_archivo, num_bloque, bloque_archivo);

	if (accion.find(COMMON_SEND_FILE) == 0) {
		this->manejadorDeArchivos->agregarArchivo(nombre_archivo, num_bloque, bloque_archivo);
	}
	else {
		if (accion.find(COMMON_DELETE_FILE) == 0) {
			this->manejadorDeArchivos->eliminarArchivo(nombre_archivo, num_bloque);
		}
	}
}

/* Metodos privados */

void ReceptorDeArchivos::parsearMensaje(const std::string &mensaje, std::string &accion,
	std::string &nombre_archivo, std::string &num_bloque, std::string &bloque_archivo) {

	// El mensaje viene en el formato "<Instruccion,Nombre_Archivo,Numero_Bloque,Bloque_Archivo"
	// Divididos por una ','
	std::string* args[4];
	std::string aux;
	std::string msj = mensaje;
	int i;
	int delim = 0;

	// Se guarda la direccion de cada argumento en el array
	args[0] = &accion;
	args[1] = &nombre_archivo;
	args[2] = &num_bloque;
	args[3] = &bloque_archivo;
	
	// Se parsea el mensaje
	for (i = 0; i < 4; i++) {
		delim = msj.find(',');
		aux = msj.substr(0, delim);
		msj.erase(0, delim + 1);
		args[i]->assign(aux.c_str());
	}
}

