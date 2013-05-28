//  
//  common_comunicador.h
//  CLASE COMUNICADOR
//  
//  Clase que implementa la interfaz de comunicación entre servidor y clientes.
//  


#include "common_comunicador.h"
#include <sstream>




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
// PRE: 'socket' es un socket por el que se desea hacer el envío y
// transmisión de mensajes
Comunicador::Comunicador(Socket *socket) : socket(socket) { }


// Emite una instrucción y sus argumentos.
// PRE: 'instruccion' es una cadena que identifica la instrucción a emitir;
// 'args' son los argumentos de dicha instrucción separadas entre si por
// un espacio.
// POST: devuelve 0 si se ha realizado el envio correctamente o -1 en caso
// de error.
int Comunicador::emitir(const std::string& instruccion, 
	const std::string& args) {
	// Armamos mensaje a enviar
	std::string msg = instruccion + " " + args + FIN_MENSAJE;
	
	// Enviamos el mensaje
	return this->socket->enviar(msg.c_str(), msg.size());
}


// Emite un mensaje.
// PRE: 'msg' es el mensaje que se desea enviar.
// POST: devuelve 0 si se ha realizado el envio correctamente o -1 en caso
// de error.
int Comunicador::emitir(const std::string& msg) {
	// Armamos mensaje a enviar
	std::string msg_n = msg + FIN_MENSAJE;
	
	// Enviamos el mensaje
	return this->socket->enviar(msg_n.c_str(), msg_n.size());
}


// Recibe una instrucción.
// POST: se almacenó la instrucción recibida en 'instruccion' y los
// argumentos en args, los cuales se encuentran separados entre si por un
// espacio. De producirse un error, 'instruccion' y 'args' queda vacíos y
// se retorna -1. En caso de exito se devuelve 0.
int Comunicador::recibir(std::string& instruccion, std::string& args) {
	// Variable auxiliar para armar mensaje
	std::stringstream msg_in;
	// Limpiamos argumentos que recibiran datos
	instruccion = "";
	args = "";

	// Recibimos de a 1 Byte hasta recibir el caractér de fin de mensaje
	while(true) {
		// Definimos buffer de 1 Byte
		char bufout[1];

		// Si se produce un error, devolvemos una instrucción vacía
		if(this->socket->recibir(bufout, 1) == -1) return -1;
		
		// Si se recibió el caractér de fin de mensaje, salimos
		if(bufout[0] == FIN_MENSAJE) break;

		// Agregamos el caractér a los datos ya recibidos
		msg_in << bufout[0];
	}

	// Paresamos instrucción y argumentos
	msg_in >> instruccion;
	getline(msg_in, args);

	// Eliminamos el espacio inicial sobrante de los argumentos
	if(args != "") args.erase(0, 1);

	return 0;
}
