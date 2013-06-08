//
//  client_configuracion.h
//  CLASE CONFIGURACION
//  


#ifndef CONFIGURACION_H
#define CONFIGURACION_H


// CONSTANTES
namespace {

	// Metadatos sobre el archivo de configuración
	const std::string CONFIG_DIR = "config/";
	const std::string CONFIG_FILENAME = "general";
	const std::string CONFIG_FILE_EXT = ".properties";

	// Parámetros configurables
	const std::string CONFIG_P_DIR = "DIRECTORIO";
	const std::string CONFIG_P_HOST = "HOSTNAME";
	const std::string CONFIG_P_PORT = "PUERTO";
	const std::string CONFIG_P_POLL = "POLLING";

	// Separadores
	const std::string CONFIG_SEPARATOR = "=";

	// Indicador de comentarios
	const std::string CONFIG_COMMENT = "#";
}




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Configuracion {
private:


public:

	// Constructor
	Configuracion();

	// Destructor
	~Configuracion();

	// Devuelve el directorio en el que se desea sincronizar.
	std::string obtenerDirectorio();

	// Devuelve el host del servidor.
	std::string obtenerHost();

	// Devuelve el puerto del servidor.
	int obtenerPuerto();

	// Devuelve el intervalo de polling en segundos.
	int obtenerIntervaloDePolling();

	// Actualiza el directorio en el que se desea sincronizar.
	void asignarDirectorio(std::string directorio);

	// Actualiza el host del servidor.
	void asignarHost(std::string host);

	// Actualiza el puerto del servidor.
	void asignarPuerto(int puerto);

	// Actualiza el intervalo de polling, el cual debe estar en segundos.
	void asignarIntervaloDePolling(int intervalo);
};

#endif
