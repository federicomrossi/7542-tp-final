//
//  client_actualizador.h
//  CLASE ACTUALIZADOR
//  


#ifndef ACTUALIZADOR_H
#define ACTUALIZADOR_H


#include <string>
#include "common_lista.h"
#include "common_logger.h"
#include "common_manejador_de_archivos.h"
#include "client_emisor.h"
#include "client_receptor.h"





/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Actualizador {
private:

	Emisor *emisor;									// Emisor de mensajes
	Receptor *receptor;								// Receptor de mensajes
	ManejadorDeArchivos *manejadorDeArchivos;		// Manejador de archivos
	int porcentajeDeActualizacion;					// Contador que sensa
													// cuanto se ha actualizado
	Logger *logger;									// Logger de eventos

public:

	// Constructor
	Actualizador(Emisor *emisor, Receptor *receptor, 
		ManejadorDeArchivos *manejadorDeArchivos, Logger *logger);

	// Destructor
	~Actualizador();

	// Inicia la actualización del directorio
	void ejecutarActualizacion();
};

#endif
