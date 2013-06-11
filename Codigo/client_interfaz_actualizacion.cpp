#include "client_interfaz_actualizacion.h"



Actualizacion::Actualizacion() {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
	
	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/actualizacion.glade");
	refBuilder->get_widget("ventanaActualizacion", this->ventana);
	refBuilder->get_widget("lblActividad", this->lblActividad);
	refBuilder->get_widget("lblDetalles", this->lblDetalles);
	refBuilder->get_widget("prgbarCompletado", this->prgbarCompletado);

	this->ventana->show_all_children();
}


Actualizacion::~Actualizacion() { }


void Actualizacion::correr(){
	Gtk::Main::run(*ventana);
}
