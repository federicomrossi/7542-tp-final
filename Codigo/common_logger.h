//  
//  common_logger.h
//  CLASE LOGGER
//  
//  Clase que escribe un en un archivo de log "<Fecha> <mensaje de log>"
//  


#ifndef LOGGER_H
#define LOGGER_H

#include <time.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "common_mutex.h"
#include "common_lock.h"

namespace {
	#define LONG_FECHA 24
}



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Logger {
private:
	Mutex* mutex;
	std::fstream* archivo;
	std::string pathArchivo;

	void crearArchivo();

public:

	// Constructores
	Logger(const std::string& nombre_archivo);

	Logger(const char* nombre_archivo);
	
	// Destructor
	~Logger();

	// Crea una nueva entrada de log.
	// PRE: 'log' es el mensaje que se insertará en el log.
	void emitirLog(const std::string& log);

	// Limpia el archivo de log existente eliminando todas las entradas
	// contenidas en él. Devuelve 0 si se eliminaron las entradas 
	// correctamente o un valor distinto de 0 sino.
	int limpiarLog();
};

#endif
