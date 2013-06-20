//  
//  client_interfaz_actualizacion.h
//  CLASE INTERFAZ DE ACTUALIZACION
//  


#include <iostream>
#include <string>
#include "client_interfaz_actualizacion.h"


// Constructor
IActualizacion::IActualizacion(Cliente *cliente) : cliente(cliente) {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
	
	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/client_actualizacion.glade");

	refBuilder->get_widget("ventanaActualizacion", this->ventana);
	this->ventana->show_all_children();
}


// Destructor
IActualizacion::~IActualizacion() { }



// Define tareas a ejecutar en el hilo.
void IActualizacion::run() {
	//Muestro configuracion actual
	Gtk::Main::run(*this->ventana);

	// Esperamos a que se termine de actualizar el directorio
	while(this->cliente->estaActualizando() && this->isActive()) {
		// INSERTAR CONTADOR PARA TIMEOUT Y APERTURA DE
		// VENTANA DE CIERRE POR TIMEOUT
		std::cout << "actualizando" << std::endl;
	}

	// Cerramos la ventana
	Gtk::Main::quit();
}
