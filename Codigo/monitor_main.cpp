//  
//  ARCHIVOS UBICUOS
//  Programa principal  MONITOR
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
//  Programa monitor que permite modificar y obtener estadisticas del servidor
//



#include <iostream>
#include "monitor_interfaz_principal.h"
#include "monitor_monitor.h"
#include "monitor_configuracion.h"

int main (int argc, char** argv) {
	// Iniciamos interfaz de la ventana principal
	Gtk::Main kit(argc, argv);

	
	// Creamos la configuracion del servidor
	Configuracion* configs = new Configuracion();
	
	// Creamos el monitor
	Monitor *monitor = new Monitor();
	// ventana principal del programa
	MenuPrincipal ventanaMonitor(monitor,configs);
	ventanaMonitor.correr();

	// Liberamos toda la memoria
	
	delete monitor;
	delete configs;
	
	return 0;
}
