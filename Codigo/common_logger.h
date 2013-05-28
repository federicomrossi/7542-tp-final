//  
//  common_logger.h
//  CLASE LOGGER
//  
//  Clase que [...]
//  


#ifndef LOGGER_H
#define LOGGER_H





/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Logger {
private:

	

public:

	// Constructor
	Logger();

	// Abre un archivo de log.
	// PRE: 'nombre_archivo' es el nombre que se le dará al archivo de log, al
	// que se le agregará la extensión ".log".
	// POST: si el archivo no existe, se creará uno nuevo. Si ya existe
	// no se modificará a este.
	void abrir(const std::string& nombre_archivo);

	// Crea una nueva entrada de log.
	// PRE: 'log' es el mensaje que se insertará en el log.
	void emitirLog(const std::string& log);

	// Limpia el archivo de log existente eliminando todas las entradas
	// contenidas en él.
	void limpiarLog();
};

#endif
