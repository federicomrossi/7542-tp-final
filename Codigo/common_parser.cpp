#include "common_parser.h"

// Parsea el mensaje separando la instruccion de sus argumentos.
// PRE: 'msg' es el mensaje que desea parsearse; 'instruccion' y 'args' son
// referencias a variables en donde se desea almacenar la instruccion y sus
// argumentos respectivamente.
void Parser::parserInstruccion(const std::string& msg, 
	std::string& instruccion, std::string& args) {
	std::stringstream msgTemp(msg);

	// Tomamos la instrucción
	msgTemp >> instruccion;
	getline(msgTemp, args);

	// Eliminamos el espacio inicial sobrante de los argumentos
	if(args != "") args.erase(0, 1);
}

// Parsea el mensaje separando los argumentos y los devuelve en una lista
// en el orden en que se leyeron. Si el mensaje esta vacio, no se modifica la lista
void Parser::dividirCadena(const std::string &msg, Lista<std::string>* args,
	char delim) {
	// Se procesa solo si el mensaje tiene contenido
	if (!msg.empty()) {
		// Variables auxiliares
		std::string m = msg;

		// posicion del delim
		size_t d;

		while(d != std::string::npos){
			d = m.find(delim);
			std::cout << m.substr(0, d) << std::endl;
			args->insertarUltimo(m.substr(0, d));
			m.erase(0, d+1);
		}
	}
}
