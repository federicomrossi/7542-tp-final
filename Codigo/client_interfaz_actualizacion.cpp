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
	
	std::cout<<"Entro a run del IActualizacion"<<std::endl;
	// Esperamos a que se termine de actualizar el directorio
	while(this->cliente->estaActualizando() && this->isActive()) {
		// INSERTAR CONTADOR PARA TIMEOUT Y APERTURA DE
		// VENTANA DE CIERRE POR TIMEOUT
		sleep(1);
	}
	std::cout<<"salgo del run del IA"<<std::endl;
	
	this->main->hide();
	this->join();
}

void IActualizacion::correr() {
	this->start();
	Gtk::Main::run(*main);
}
