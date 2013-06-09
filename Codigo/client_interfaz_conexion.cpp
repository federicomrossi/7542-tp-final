#include <iostream>
#include <string>
#include "client_configuracion.h"
#include "client_interfaz_conexion.h"



Conexion::Conexion(Cliente *cliente) : cliente(cliente) {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
	
	
	
	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/conexion.glade");
	this->icono = gtk_status_icon_new_from_icon_name("trayIcon");
	gtk_status_icon_set_from_file(this->icono,"logo_au.png");
	

	refBuilder->get_widget("conexion", this->main); // linkeo el form
	
	refBuilder->get_widget("usuarioTxt", this->usuarioTextBox);
	refBuilder->get_widget("passTxt", this->passTextBox);

	refBuilder->get_widget("conectar", this->botonConectar);
	refBuilder->get_widget("lblError", this->lblError);


	this->botonConectar->signal_clicked().connect(sigc::mem_fun(*this, &Conexion::on_buttonConectar_clicked)); 
	refBuilder->get_widget("Salir", this->botonSalir);
	this->botonSalir->signal_clicked().connect(sigc::mem_fun(*this, &Conexion::on_buttonSalir_clicked));

	main->show_all_children();
}


void Conexion::on_buttonConectar_clicked() {
	// Deshabilitamos objetos de la ventana
	this->botonConectar->set_sensitive(false);
	this->usuarioTextBox->set_sensitive(false);
	this->passTextBox->set_sensitive(false);
	
	// Creamos objeto para obtener la configuración
	Configuracion config;

	cliente->especificarNombreHost(config.obtenerHost());
	cliente->especificarPuerto(config.obtenerPuerto());
	cliente->especificarDirectorio(config.obtenerDirectorio());

	std::string user = this->usuarioTextBox->get_text();
	std::string pass = this->passTextBox->get_text();
	
	// Iniciamos conexión
	int r = cliente->conectar(user, pass);

	if(r == 1) {
		cliente->iniciarSincronizacion(config.obtenerIntervaloDePolling());
		this->lblError->set_text("");
		
	}
	else if(r == 0) {
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
	else if(r == -1) {
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

}


void Conexion::correr(){
	Gtk::Main::run(*main);
}

Conexion::~Conexion() { }

