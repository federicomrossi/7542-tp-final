#include <iostream>
#include <string>
#include "client_configuracion.h"

#include "client_interfaz_conexion.h"



Conexion::Conexion(Cliente *cliente, Configuracion* clienteConfig) : cliente(cliente), clienteConfig(clienteConfig) {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

	
	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/client_conexion.glade");


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

}


void Conexion::on_buttonConectar_clicked() {
	// Deshabilitamos objetos de la ventana
	this->botonConectar->set_sensitive(false);
	this->usuarioTextBox->set_sensitive(false);
	this->passTextBox->set_sensitive(false);
	this->lblError->set_text("");
	
	// Obtenemos la configuracion actual del cliente

	cliente->especificarNombreHost(this->clienteConfig->obtenerHost());
	cliente->especificarPuerto(this->clienteConfig->obtenerPuerto());
	cliente->especificarDirectorio(this->clienteConfig->obtenerDirectorio());

	std::string user = this->usuarioTextBox->get_text();
	std::string pass = this->passTextBox->get_text();
	
	// Iniciamos conexión
	this->estadoConexion = cliente->conectar(user, pass);

	if(this->estadoConexion == 1) {
		// Abrimos ventana de actualización
		this->main->set_sensitive(false);
		this->lblError->set_text("");


		 this->lblError->set_text("Sincronizando datos");
		
		this->cliente->iniciarSincronizacion(
			this->clienteConfig->obtenerIntervaloDePolling());
		this->start();
		
		 
			// Habilitamos ventana luego de la actualización
		this->main->set_sensitive(true);
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
	this->estadoConexion = 0;
	this->join();
	this->cliente->desconectar();
	
	Gtk::Main::quit();
}

void Conexion::on_menuPref_activate() {


	IConfiguracion ventanaSettings(this->clienteConfig, this->estadoConexion);
	ventanaSettings.correr();

	this->main->set_sensitive(true);
}

void Conexion::on_menuSalir_activate() {
	this->estadoConexion = 0;
	this->join();
	this->cliente->desconectar();
	
	Gtk::Main::quit();

}

void Conexion::run() {

	
while(this->cliente->estaSincronizando() == true && this->estadoConexion == 1 ){
		this->lblError->set_text("Conectado al servidor");
		sleep(1);
}	

	this->usuarioTextBox->set_sensitive(true);
	this->passTextBox->set_sensitive(true);
	this->lblError->set_text("Se cayó la conexion, ingrese para sincronizar");
	this->botonConectar->set_sensitive(true);
	this->estadoConexion = 0;
	this->cliente->detenerSincronizacion();


	
}

void Conexion::correr(){
	
	Gtk::Main::run(*main);
}

Conexion::~Conexion() { }

