#include <iostream>
#include <fstream>
#include <stdlib.h>



namespace {
	// Constantes que definen los comandos válidos
	const std::string CMD_SALIR = "q";
}




/* ****************************************************************************
* PROGRAMA PRINCIPAL
* ***************************************************************************/


int main(int argc, char* argv[]) {
	
	std::cout << "INICIO" << std::endl;

	std::string comando;

	// Esperamos a que se indique la finalización de la ejecución
	while(comando != CMD_SALIR)
	getline(std::cin, comando);

	std::cout << "FIN" << std::endl;

	return 0;
}