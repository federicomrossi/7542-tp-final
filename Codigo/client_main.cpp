//  
//  ARCHIVOS UBICUOS
//  Programa principal del CLIENTE
//  
//  ***************************************************************************
//  
//  Facultad de Ingeniería - UBA
//  75.42 Taller de Programación I
//  Trabajo Práctico N°5
//  
//  ALUMNOS:
//	Belén Beltran (91718) - belubeltran@gmail.com
//	Fiona Gonzalez Lisella (91454) - fgonzalezlisella@gmail.com
//	Federico Martín Rossi (92086) - federicomrossi@gmail.com
//  
//  ***************************************************************************
//
//  Programa cliente que permite conectarse a un servidor para [...]
//



#include <iostream>
#include "client_interfaz_conexion.h"
#include "client_cliente.h"
#include "client_interfaz_trayIcon.h"



int main (int argc, char** argv) {
	try{
		// Iniciamos interfaz de la ventana principal
		Gtk::Main kit(argc, argv);

		// Creamos el cliente
		Cliente *cli = new Cliente;

		// Creamos la configuracion del cliente
		Configuracion* configs = new Configuracion();

		// ventana principal del programa
		Conexion ventanaConexion(cli, configs);
		ventanaConexion.correr();

		// Liberamos toda la memoria
		delete configs;
		delete cli;
	}
	catch(char const * e) {
		std::cout << e << std::endl;
	}

	return 0;
}
