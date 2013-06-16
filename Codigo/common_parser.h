// Parser de mensajes

#ifndef COMMON_PARSER_H
#define COMMON_PARSER_H

#include <string>
#include <sstream>
#include "common_lista.h"

//DEBUG
#include <iostream>

class Parser {
public:
	// Parsea el mensaje separando la instruccion de sus argumentos.
	// PRE: 'msg' es el mensaje que desea parsearse; 'instruccion' y 'args' son
	// referencias a variables en donde se desea almacenar la instruccion y sus
	// argumentos respectivamente.
	static void parserInstruccion(const std::string& msg, 
		std::string& instruccion, std::string& args);

	// Parsea el mensaje separando los argumentos y los devuelve en una lista
	// en el orden en que se leyeron
	static void parserArgumentos(const std::string &msg, Lista<std::string>* args,
		char delim);

};

#endif
