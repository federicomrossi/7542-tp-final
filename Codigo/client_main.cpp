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



#include <iostream>
#include "client_interfaz_conexion.h"
#include "client_cliente.h"
#include "client_interfaz_trayIcon.h"



int main (int argc, char** argv) {
	// Iniciamos interfaz de la ventana principal
	Gtk::Main kit(argc, argv);

	// Creamos el cliente
	Cliente *cli = new Cliente;
	
	Conexion ventanaConexion(cli);
	ventanaConexion.correr();
	
	
	
	//TrayIcon trayConectado(cli);
	//trayConectado.correr();

	// Liberamos memoria usada por el cliente
	delete cli;

	return 0;
}
