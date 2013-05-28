//  
//  common_comunicador.h
//  CLASE COMUNICADOR
//  
//  Clase que implementa la interfaz de comunicación entre servidor y clientes.
//  


#ifndef COMUNICADOR_H
#define COMUNICADOR_H



#include <string>
#include "common_socket.h"
#include "common_protocolo.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Comunicador {
private:

	Socket *socket;				// Socket de comunicación

public:

	// Constructor
	// PRE: 'socket' es un socket por el que se desea hacer el envío y
	// transmisión de mensajes
	explicit Comunicador(Socket *socket);

	// Emite una instrucción.
	// PRE: 'instruccion' es una cadena que identifica la instrucción a emitir;
	// 'args' son los argumentos de dicha instrucción separadas entre si por
	// un espacio.
	// POST: devuelve 0 si se ha realizado el envio correctamente o -1 en caso
	// de error.
	int emitir(const std::string& instruccion, const std::string& args);

	// Emite un mensaje.
	// PRE: 'msg' es el mensaje que se desea enviar.
	// POST: devuelve 0 si se ha realizado el envio correctamente o -1 en caso
	// de error.
	int emitir(const std::string& msg);

	// Recibe una instrucción.
	// POST: se almacenó la instrucción recibida en 'instruccion' y los
	// argumentos en args, los cuales se encuentran separados entre si por un
	// espacio. De producirse un error, 'instruccion' y 'args' queda vacíos y
	// se retorna -1. En caso de exito se devuelve 0.
	int recibir(std::string& instruccion, std::string& args);
};

#endif
