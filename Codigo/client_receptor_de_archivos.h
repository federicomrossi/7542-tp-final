//
//  client_receptor_de_archivos.h
//  CLASE RECEPTORDEARCHIVOS
//  


#ifndef RECEPTOR_DE_ARCHIVOS_H
#define RECEPTOR_DE_ARCHIVOS_H


#include <string>
#include "client_manejador_de_archivos.h"
#include "common_protocolo.h"
class Mutex;
class Lock;



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ReceptorDeArchivos {
private:

	Mutex m;							// Mutex
	ManejadorDeArchivos *manejadorDeArchivos;		// Manejador de archivos
	
	void parsearMensaje(const std::string &mensaje, std::string &accion,
		std::string &nombre_archivo, std::string &num_bloque, std::string &bloque_archivo);

public:

	// Constructor
	ReceptorDeArchivos(ManejadorDeArchivos *unManejador);

	// Destructor
	~ReceptorDeArchivos();
	
	// Debe enviar al manejador de archivos el archivo recibido en el mensaje
	// Llega el archivo con COMMON_SEND_FILE o COMMON_DELETE_FILE al comienzo
	// para saber si hay que agregar un bloque
	// o eliminar un archivo del directorio

	// Formato de archivo: "<Instruccion,Nombre_Archivo,Numero_Bloque,Bloque_Archivo>".
	// Para hacer referencia a todo el archivo, Numero_Bloque = WHOLE_FILE
	void recibirArchivo(std::string &mensaje);

	// Se encarga de procesar la eliminación de un archivo
	void eliminarArchivo(std::string& nombreArchivo);
};

#endif
