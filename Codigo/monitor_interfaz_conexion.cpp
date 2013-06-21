#include <iostream>
#include <string>
#include "monitor_configuracion.h"
#include "monitor_interfaz_conexion.h"



Conexion::Conexion(Receptor *receptor, Configuracion* receptorConfig) : receptor(receptor), receptorConfig(receptorConfig) {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

	
	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/monitor_conexion.glade");


	refBuilder->get_widget("conexion", this->main); // linkeo el form
		
	refBuilder->get_widget("usuarioTxt", this->usuarioTextBox);
	refBuilder->get_widget("passTxt", this->passTextBox);

	refBuilder->get_widget("conectar", this->botonConectar);
	refBuilder->get_widget("lblError", this->lblError);
	refBuilder->get_widget("preferencias", this->menuPref);
	refBuilder->get_widget("msalir", this->menuSalir);
	refBuilder->get_widget("Salir", this->botonSalir);

	this->botonConectar->signal_clicked().connect(sigc::mem_fun(*this, &Conexion::on_buttonConectar_clicked)); 
	
	this->botonSalir->signal_clicked().connect(sigc::mem_fun(*this, &Conexion::on_buttonSalir_clicked));
	this->menuPref->signal_activate().connect(sigc::mem_fun(*this, &Conexion::on_menuPref_activate));
	this->menuSalir->signal_activate().connect(sigc::mem_fun(*this, &Conexion::on_menuSalir_activate));

	main->show_all_children();
	this->flagSalida = 0;

}


void Conexion::on_buttonConectar_clicked() {
	// Deshabilitamos objetos de la ventana
	this->botonConectar->set_sensitive(false);
	this->usuarioTextBox->set_sensitive(false);
	this->passTextBox->set_sensitive(false);
	
	// Obtenemos la configuracion actual del cliente

	receptor->especificarNombreHost(this->receptorConfig->obtenerHost());
	receptor->especificarPuerto(this->receptorConfig->obtenerPuerto());
	receptor->especificarTiempo(this->receptorConfig->obtenerTiempo());
	
	std::string user = this->usuarioTextBox->get_text();
	std::string pass = this->passTextBox->get_text();
	
	// Iniciamos conexión
	this->estadoConexion = receptor->conectar(user, pass);

	if(this->estadoConexion == 1) {
		
		this->main->set_sensitive(false);
		this->main->hide();
		// se puede incluir una ventana de actualizacion
	}
	else if(this->estadoConexion == 0) {
		// Mostramos mensaje de error en ventana
		this->lblError->set_text("Usuario y/o contraseña inválidos");
		this->lblError->set_visible(true);

		// Borramos el contenido del password para ser nuevamente escrito
		this->passTextBox->set_text("");

		// Habilitamos objetos de la ventana
		this->botonConectar->set_sensitive(true);
		this->usuarioTextBox->set_sensitive(true);
		this->passTextBox->set_sensitive(true);
	}
	else if(this->estadoConexion == -1) {
		// Mostramos mensaje de error en ventana
		this->lblError->set_text("Falló la conexión con el servidor.");
		this->lblError->set_visible(true);

		// Habilitamos objetos de la ventana
		this->botonConectar->set_sensitive(true);
		this->usuarioTextBox->set_sensitive(true);
		this->passTextBox->set_sensitive(true);
	}
}

void Conexion::on_buttonSalir_clicked() {
	Gtk::Main::quit();
	this->flagSalida = 1;
}

void Conexion::on_menuPref_activate() {
	IConfiguracion ventanaSettings(this->receptorConfig, this->estadoConexion);
	ventanaSettings.correr();
	this->main->set_sensitive(true);

}


void Conexion::on_menuSalir_activate() {
	Gtk::Main::quit();
	this->flagSalida = 1;

}


int Conexion::correr(){
	Gtk::Main::run(*main);
	return this->flagSalida;
}

Conexion::~Conexion() { }

