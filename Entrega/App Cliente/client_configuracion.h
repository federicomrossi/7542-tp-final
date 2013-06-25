//
//  client_configuracion.h
//  CLASE CONFIGURACION
//  


#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include "common_archivoTexto.h"

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
	ArchivoTexto* Archivo;

public:

	// Constructor
	Configuracion();

	// Destructor
	~Configuracion();

	// Devuelve el valor especifico que se necesita
	std::string getInfo(std :: string &cadena);

	// Devuelve el directorio en el que se desea sincronizar.
	std::string obtenerDirectorio();

	// Devuelve el host del servidor.
	std::string obtenerHost();

	// Devuelve el puerto del servidor.
	int obtenerPuerto();

	// Devuelve el intervalo de polling en segundos.
	int obtenerIntervaloDePolling();
	
	// Guarda cambios realizados sobre la configuracion.
	void guardarCambios(string host,string puerto,string dir, string polling);

};

#endif
