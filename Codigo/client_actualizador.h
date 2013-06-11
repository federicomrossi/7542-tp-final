//
//  client_actualizador.h
//  CLASE ACTUALIZADOR
//  


#ifndef ACTUALIZADOR_H
#define ACTUALIZADOR_H


#include <string>
#include "common_archivo.h"
#include "common_lista.h"
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
	int porcentajeDeActualizacion;					// Contador que sensa
													// cuanto se ha actualizado

	// Parsea el mensaje separando la instruccion de sus argumentos.
	// PRE: 'msg' es el mensaje que desea parsearse; 'instruccion' y 'args' son
	// referencias a variables en donde se desea almacenar la instruccion y sus
	// argumentos respectivamente.
	void parserMensaje(const std::string& msg, std::string& instruccion,
		std::string& args);

	// Parsea los datos de un archivo
	// PRE: 'args' es la cadena que contiene los datos separados por una coma: 
	// [NOMBRE],[HASH],[FECHA_MODIFICACION]; 'archivo' es un puntero al objeto
	// Archivo en donde se almacenarán dichos datos.
	void parserArchivo(const std::string args, Archivo *archivo);

	// Parsea la lista de archivos.
	// PRE: 'listaDeArchivos' es una lista que contiene datos de archivos 
	// agrupados en orden: nombre, hash, fecha; 'lista' es una lista de 
	// Archivos en la que se almacenarán los objetos de tipo Archivo producto
	// del parseo.
	void parserArchivos(const std::string& listaDeArchivos, 
		Lista< Archivo > *lista);

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
