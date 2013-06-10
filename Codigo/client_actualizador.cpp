//
//  client_actualizador.h
//  CLASE ACTUALIZADOR
//  


#include <string>
#include <sstream> 
#include "common_protocolo.h"
#include "common_archivo.h"
#include "client_actualizador.h"

// DEBUG
#include <iostream>
// END DEBUG





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Actualizador::Actualizador(Emisor *emisor, Receptor *receptor, 
		ManejadorDeArchivos *manejadorDeArchivos) : emisor(emisor),
		receptor(receptor), manejadorDeArchivos(manejadorDeArchivos) { }


// Destructor
Actualizador::~Actualizador() { }


// Inicia la recepción
void Actualizador::ejecutarActualizacion() {
	// Solicitamos la lista de archivos del servidor
	this->emisor->ingresarMensajeDeSalida(C_GET_FILES_LIST);

	std::string instruccion, args;

	// Esperamos a recibir la lista de archivos desde el servidor
	while(instruccion != S_FILES_LIST) {
		std::string msg = this->receptor->obtenerMensajeDeEntrada();
		this->parserMensaje(msg, instruccion, args);
	}
}


/*
 * IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */


// Parsea el mensaje separando la instruccion de sus argumentos.
// PRE: 'msg' es el mensaje que desea parsearse; 'instruccion' y 'args' son
// referencias a variables en donde se desea almacenar la instruccion y sus
// argumentos respectivamente.
void Actualizador::parserMensaje(const std::string& msg, 
	std::string& instruccion, std::string& args) {
	std::stringstream msgTemp(msg);

	// Tomamos la instrucción
	msgTemp >> instruccion;
	getline(msgTemp, args);

	// Eliminamos el espacio inicial sobrante de los argumentos
	if(args != "") args.erase(0, 1);
}
