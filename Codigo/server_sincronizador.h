//  
//  server_sincronizador.h
//  CLASE SINCRONIZADOR
//  


#ifndef SINCRONIZADOR_H
#define SINCRONIZADOR_H


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

	// Parsea el mensaje separando la instruccion de sus argumentos.
	// PRE: 'msg' es el mensaje que desea parsearse; 'instruccion' y 'args' son
	// referencias a variables en donde se desea almacenar la instruccion y sus
	// argumentos respectivamente.
	void parserMensaje(const std::string& msg, std::string& instruccion,
		std::string& args);

	// Parsea los datos de un archivo
	// PRE: 'args' es la cadena que contiene los datos separados por una coma: 
	// [NOMBRE],[NUM_BLOQUE],[BLOQUE],[HASH],[FECHA]; 'archivo' es un puntero 
	// al objeto Archivo en donde se almacenarán dichos datos.
	// void parserArchivo(const std::string argumentos, Archivo *archivo);

public:

	// Constructor
	Sincronizador(Receptor *receptor, Emisor *emisor, 
		ManejadorDeArchivos *manejadorDeArchivos);

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
