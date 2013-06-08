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
//	Fiona Gonzalez Lisella (91454) - dynamo89@gmail.com
//	Federico Martín Rossi (92086) - federicomrossi@gmail.com
//  
//  ***************************************************************************



#include <iostream>
#include <string>
#include <sstream>

#include "common_convertir.h"
#include "common_logger.h" 
#include "client_mainCliente.h"
#include "client_cliente.h"
#include "client_configuracion.h"




/* ****************************************************************************
 * PROGRAMA PRINCIPAL
 * ***************************************************************************/


MainCliente :: MainCliente() { }


int MainCliente :: conectar(string user, string pass) {
		
	// Creamos objeto para obtener la configuración
	Configuracion config;

	// Creamos el cliente
	Cliente cliente(config.obtenerHost(), config.obtenerPuerto(),
		config.obtenerDirectorio());

	// Iniciamos su conexion
	int result = cliente.conectar(user, pass);

	if (result == 1) {
		cliente.iniciarSincronizacion(config.obtenerIntervaloDePolling());
		cout << "se inicia sincro"<<endl;
		return 1;
	}		
	//Falta validar salida erronea

	return 0;
}


MainCliente :: ~MainCliente() {

}
