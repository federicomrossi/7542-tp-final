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
//  Programa cliente que permite conectarse a un servidor para [...]
//  
//  
//  FORMA DE USO
//  ============
//
//  Deberá ejecutarse el programa en la línea de comandos de la siguiente 
//  manera:
//  
//  	# ./client [NOMBRE-HOST] [PUERTO]
//
//  donde,
//
//  	NOMBRE-HOST: es el nombre del host donde está el servidor;
//  	PUERTO: es el puerto donde el servidor está escuchando
//
//  



#include <iostream>
#include <string>
#include <sstream>
#include "client_cliente.h"
#include "common_convertir.h"




/* ****************************************************************************
 * PROGRAMA PRINCIPAL
 * ***************************************************************************/


int main(int argc, char* argv[]) {
	// Corroboramos cantidad de argumentos
	if(argc != 2) {
		std::cerr << "ERROR: cantidad incorrecta de argumentos." << std::endl;
		return 1;
	}

	// Datos para la conexión
	int puerto;
	std::string nombreHost;
	
	// Variables auxiliares
	std::string sPuerto;
	std::stringstream argumento(argv[1]);
	
	// Obtenemos dirección IP o nombre del host
	getline(argumento, nombreHost, ':');

	// Obtenemos el puerto 
	getline(argumento, sPuerto, ':');
	puerto = Convertir::stoi(sPuerto);

	// Creamos el cliente
	Cliente cliente(nombreHost, puerto);

	// Iniciamos su ejecución
	cliente.ejecutar();

	return 0;
}
