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
//  Programa monitor que permite modificar y obtener estadisticas del servidor
//


#include <iostream>
#include "common_thread.h"
#include "monitor_interfaz_principal.h"
#include "monitor_receptorDatos.h"
#include "monitor_monitor.h"
#include "monitor_configuracion.h"
#include "monitor_interfaz_conexion.h"


int main (int argc, char** argv) {
	// Iniciamos interfaz de la ventana principal
	Gtk::Main kit(argc, argv);
	
	// Creamos la configuracion del servidor
	Configuracion* configs = new Configuracion();

	// Creamos el receptor de datos que se comunica con el servidor
	Receptor* receptor = new Receptor();

	// Creamos el monitor
	Monitor* monitor = new Monitor(receptor);

	//Iniciamos ventana de conexion

	Conexion ventanaConexion(receptor,configs);
	if (ventanaConexion.correr() == 1) return 0;
	std::cout<<"vuelvo a main "<<endl;
	// ventana principal del programa
	MenuPrincipal ventanaMonitor(monitor,configs);
	ventanaMonitor.correr();

	// Liberamos toda la memoria
	
	delete monitor;
	delete configs;
	delete receptor;
	
	return 0;
}