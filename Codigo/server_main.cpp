//  
//  ARCHIVOS UBICUOS
//  Programa principal del SERVIDOR
//  
//  ***************************************************************************
//  
//  Facultad de Ingeniería - UBA
//  75.42 Taller de Programación I
//  Trabajo Práctico N°5
//  
//  ALUMNOS:
//	Belén Beltran (91718) - belubeltran@gmail.com
//	Fiona Gonzalez Lisella () - dynamo89@gmail.com
//	Federico Martín Rossi (92086) - federicomrossi@gmail.com
//  
//  ***************************************************************************
//
//  Programa servidor el cual se encarga de [...]
//  
//  
//  
//  FORMA DE USO
//  ============
//
//  Deberá ejecutarse el programa en la línea de comandos de la siguiente 
//  manera:
//  
//  	# ./server [PUERTO]
//
//  donde,
//
//  	PUERTO: es el servidor donde deberá escuchar el servidor;
//
//  



#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "server_servidor.h"



namespace {
	// Constantes que definen los comandos válidos
	const std::string CMD_SALIR = "q";
}




/* ****************************************************************************
 * PROGRAMA PRINCIPAL
 * ***************************************************************************/


int main(int argc, char* argv[]) {
	// Corroboramos cantidad de argumentos
	// if(argc != 5) {
	// 	std::cerr << "ERROR: cantidad incorrecta de argumentos." << std::endl;
	// 	return 1;
	// }

	// Creamos el servidor
	Servidor *servidor = new Servidor(atoi(argv[1]));

	try {
		// Iniciamos servidor
		servidor->iniciar();
		std::cout << "Inicio server"<< std::endl;
	}
	catch(char const * e) {
		std::cerr << e << std::endl;
		delete servidor;
		return 1;
	}

	std::string comando;
	std::cout << "Esperando 'q' para salir.." << std::endl;
	// Esperamos a que se indique la finalización de la ejecución
	while(comando != CMD_SALIR)
		getline(std::cin, comando);

	// DEBUG
	std::cout << "Saliendo y cerrando el servidor.." << std::endl;
	// END DEBUG

	servidor->detener();
	servidor->join();

	// DEBUG
	std::cout << "Se detuvo el servidor.." << std::endl;
	// END DEBUG

	// delete terminal;
	delete servidor;

	return 0;
}
