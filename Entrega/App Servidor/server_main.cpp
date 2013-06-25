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
//  	# ./server
//
//  Para detener la ejecución del servidor se debe presionar la tecla 'q'
//  seguido de ENTER.
//  



#include <iostream>
#include "server_configuracion.h"
#include "server_servidor.h"



namespace {
	// Constantes que definen los comandos válidos
	const std::string CMD_SALIR = "q";
}




/* ****************************************************************************
 * PROGRAMA PRINCIPAL
 * ***************************************************************************/


int main(int argc, char** argv) {
	// Corroboramos cantidad de argumentos
	if(argc > 1) {
		// Enviamos a log
		std::cerr << "ERROR: cantidad incorrecta de argumentos." << std::endl;
		return 1;
	}

	// Creamos módulos utilizados
	Servidor servidor;
	Configuracion configuracion;
		
	// Mensaje de log
	std::cout << "Iniciando servidor AU..." << std::endl;
	std::cout.flush();

	// Iniciamos servidor
	if(!servidor.iniciar(configuracion.obtenerPuerto())) {
		// Enviamos a log
		std::cerr << "ERROR: No ha sido posible iniciar el servidor." 
			<< std::endl;

		return 0;
	}

	// Mensaje de log
	std::cout << "Servidor corriendo. Presione 'q' para salir." << std::endl;
	std::cout.flush();

	// Variable auxiliar
	std::string comando;

	// Esperamos a que se indique la finalización de la ejecución
	while(comando != CMD_SALIR)
		getline(std::cin, comando);


	// Mensaje de log
	std::cout << "Deteniendo el servidor..." << std::endl;
	std::cout.flush();

	// Damos orden de detener servidor
	servidor.detener();
	servidor.join();

	// Mensaje de log
	std::cout << "Servidor detenido. " << std::endl;
	std::cout.flush();

	return 0;
}
