#include <iostream>
#include <string>

#include "client_interfaz_configuracion.h"
#include "common_convertir.h"




IConfiguracion::IConfiguracion(Configuracion *config, int flag) {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
	
	
	
	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/client_configuracion.glade");


	refBuilder->get_widget("main", this->main); // linkeo el form
	
	refBuilder->get_widget("host", this->host);
	refBuilder->get_widget("port", this->puerto);
	refBuilder->get_widget("dir", this->directorio);
	refBuilder->get_widget("polling", this->iPolling);

	refBuilder->get_widget("guardar", this->botonGuardar);
	refBuilder->get_widget("cancelar", this->botonCancelar);


	this->botonGuardar->signal_clicked().connect(sigc::mem_fun(*this, &IConfiguracion::on_buttonGuardar_clicked)); 
	this->botonCancelar->signal_clicked().connect(sigc::mem_fun(*this, &IConfiguracion::on_buttonCancelar_clicked));

	this->flag = flag;
	main->show_all_children();
}


void IConfiguracion::on_buttonGuardar_clicked() {

	//obtengo cada valor almacenado en los textBox
	
	string unHost = this->host->get_text();
	string unPuerto = this->puerto->get_text();
	string unDir = this->directorio->get_text();
	string unPolling = this->iPolling->get_text();
	
	this->config->guardarCambios(unHost, unPuerto, unDir, unPolling);

	this->main->hide();
		
}

void IConfiguracion::on_buttonCancelar_clicked() {
	// No hago nada, retorno sin cambios en el archivo de settings.
	this->main->hide();

}


void IConfiguracion::correr() {
	
	//cargo los textBox con info
	string auxPuerto = Convertir::itos(this->config->obtenerPuerto());
	string auxPolling = Convertir::itos(this->config->obtenerIntervaloDePolling());
	
	this->host->set_text(this->config->obtenerHost()); 
	this->puerto->set_text(auxPuerto); 
	this->directorio->set_text(this->config->obtenerDirectorio());
	this->iPolling->set_text(auxPolling);
	
	if (this->flag == 1) {
		this->host->set_sensitive(false);
		this->puerto->set_sensitive(false);
		this->iPolling->set_sensitive(false);
		this->directorio->set_sensitive(false);
	}
	//Muestro configuracion actual
	Gtk::Main::run(*main);
		
	
}

IConfiguracion::~IConfiguracion() { }

