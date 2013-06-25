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

	refBuilder->get_widget("ventanaActualizacion", this->main);
	this->main->show_all_children();
}



IActualizacion::~IActualizacion() { }


// Define tareas a ejecutar en el hilo.
void IActualizacion::run() {
	this->main->show();

	// Esperamos a que se termine de actualizar el directorio
	while(this->isActive()) {
		sleep(2);
	}

	
	this->main->hide();
	this->stop();
}

void IActualizacion::correr() {
	Gtk::Main::run(*main);
	

}

void IActualizacion::detener() {
	this->stop();
}
