//  
//  server_sincronizador.h
//  CLASE SINCRONIZADOR
//  


#ifndef SINCRONIZADOR_H
#define SINCRONIZADOR_H


#include "common_logger.h"
#include "common_thread.h"
#include "server_emisor.h"
#include "server_receptor.h"
#include "common_manejador_de_archivos.h"





/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Sincronizador : public Thread {
private:

	Receptor *receptor;								// Receptor
	Emisor *emisor;									// Emisor
	ManejadorDeArchivos *manejadorDeArchivos;		// Manejador
	Logger *logger;									// Logger de eventos

public:

	// Constructor
	Sincronizador(Receptor *receptor, Emisor *emisor, 
		ManejadorDeArchivos *manejadorDeArchivos, Logger *logger);

	// Destructor
	~Sincronizador();

	// Inicia el sincronizador.
	void iniciar();

	// Detiene al sincronizador.
	void detener();

	// Define tareas a ejecutar en el hilo.
	// Toma los mensajes que van llegando, los procesa y responde a clientes.
	virtual void run();
};

#endif
