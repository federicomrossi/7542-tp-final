#include <string>
#include "common_logger.h"


// Constantes
namespace {
	const std::string LOG_EXTENSION = ".log";
}




Logger::Logger(const std::string& nombre_archivo) {
	this->pathArchivo = nombre_archivo + LOG_EXTENSION;
	crearArchivo();
	this->mutex = new Mutex;
}

Logger::Logger(const char* nombre_archivo) {
	this->pathArchivo.assign(nombre_archivo);
	this->pathArchivo += LOG_EXTENSION;
	crearArchivo();
	this->mutex = new Mutex;
}

Logger::~Logger() {
	if (this->mutex) {
		delete(this->mutex);
		this->mutex = NULL;
	}
	if (this->archivo) {
		if (this->archivo->is_open())
			this->archivo->close();
		delete(this->archivo);
		this->archivo = NULL;
	}
}

void Logger::emitirLog(const std::string& log) {
	Lock lock(*this->mutex);
	// Se obtiene la hora y fecha actual
	time_t tiempo;
	time(&tiempo);
	struct tm* infoTiempo;
	infoTiempo = localtime(&tiempo);
	// Se va al final del archivo
	this->archivo->seekp(0, std::ios_base::end);
	// Se escribe en el log la fecha y luego el mensaje
	this->archivo->write(asctime(infoTiempo), LONG_FECHA);
	this->archivo->put(' ');
	this->archivo->write(log.c_str(), log.length());
	this->archivo->put('\n');
}

int Logger::limpiarLog() {
	Lock lock(*this->mutex);
	int error;
	// Se cierra y se intenta eliminar el archivo
	if (this->archivo->is_open())	
		this->archivo->close();
	delete(this->archivo);
	this->archivo = NULL;
	error = remove(pathArchivo.c_str());
	if (error == 0)
		// Se vuelve a crear el archivo con el mismo nombre
		crearArchivo();
	return error;
}

// Metodos privados

void Logger::crearArchivo() {
	this->archivo = new std::fstream(pathArchivo.c_str(), std::ios_base::in 
		| std::ios_base::out | std::ios_base::app);
  	if (!archivo->is_open()) {
		//Se crea el archivo
		archivo->clear();
		archivo->open(pathArchivo.c_str(), std::fstream::out);
		archivo->close();
    		// Se reabre para lectura
    		archivo->open(pathArchivo.c_str(), std::ios_base::in 
			| std::ios_base::out | std::ios_base::app);
	}
}
