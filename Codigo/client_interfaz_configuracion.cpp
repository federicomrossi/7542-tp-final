#include <iostream>
#include <string>

#include "client_interfaz_configuracion.h"




IConfiguracion::IConfiguracion(Configuracion *config) {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
	
	
	
	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/configuracion.glade");


	refBuilder->get_widget("main", this->main); // linkeo el form
	
	refBuilder->get_widget("host", this->host);
	refBuilder->get_widget("port", this->puerto);
	refBuilder->get_widget("dir", this->directorio);
	refBuilder->get_widget("polling", this->iPolling);

	refBuilder->get_widget("guardar", this->botonGuardar);
	refBuilder->get_widget("cancelar", this->botonCancelar);


	this->botonGuardar->signal_clicked().connect(sigc::mem_fun(*this, &IConfiguracion::on_buttonGuardar_clicked)); 
	this->botonCancelar->signal_clicked().connect(sigc::mem_fun(*this, &IConfiguracion::on_buttonCancelar_clicked));

	main->show_all_children();
}


void IConfiguracion::on_buttonGuardar_clicked() {

	// obtengo cada valor almacenado en los textBox
	//Validamos?¿?¿?¿?¿?¿ no
	//this->config->guardarCambios(string unHost, string unPuerto, string unDir, string polling);
		
}

void IConfiguracion::on_buttonCancelar_clicked() {
	// No hago nada, retorno sin cambios en el archivo de settings.
	Gtk::Main::quit();

}


void IConfiguracion::correr() {
	
	//cargo los textBox con info
	this->host->set_text(this->config->obtenerHost()); 
	//this->puerto->set_text(this->config->obtenerPuerto()); // hay q ver como lo pasamos!!!!!!
	//this->directorio->set_text((string)this->config->obtenerDirectorio());
	//this->iPolling->set_text(this->config->obtenerIntervaloDePolling());
	//Muestro configuracion actual
	Gtk::Main::run(*main);
}

IConfiguracion::~IConfiguracion() { }

