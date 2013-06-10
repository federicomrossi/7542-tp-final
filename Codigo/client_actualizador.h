//
//  client_actualizador.h
//  CLASE ACTUALIZADOR
//  


#ifndef ACTUALIZADOR_H
#define ACTUALIZADOR_H


#include "client_emisor.h"
#include "client_receptor.h"
#include "client_manejador_de_archivos.h"





/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Actualizador {
private:

	Emisor *emisor;									// Emisor de mensajes
	Receptor *receptor;								// Receptor de mensajes
	ManejadorDeArchivos *manejadorDeArchivos;		// Manejador de archivos

	// Parsea el mensaje separando la instruccion de sus argumentos.
	// PRE: 'msg' es el mensaje que desea parsearse; 'instruccion' y 'args' son
	// referencias a variables en donde se desea almacenar la instruccion y sus
	// argumentos respectivamente.
	void parserMensaje(const std::string& msg, std::string& instruccion,
		std::string& args);

public:

	// Constructor
	Actualizador(Emisor *emisor, Receptor *receptor, 
		ManejadorDeArchivos *manejadorDeArchivos);

	// Destructor
	~Actualizador();

	// Inicia la actualización del directorio
	void ejecutarActualizacion();
};

#endif
