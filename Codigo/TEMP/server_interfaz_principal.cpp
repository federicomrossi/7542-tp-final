#include <iostream>
#include <string>
#include "server_interfaz_principal.h"
#include "server_interfaz_configuracion.h"



MenuPrincipal::MenuPrincipal(Servidor *servidor, Configuracion *config) : servidor(servidor), serverConfig(config) {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

	
	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/server_principal.glade");
	refBuilder->get_widget("main", this->main); // linkeo el form

	// Etiquetas	
	refBuilder->get_widget("lblEstado", this->estado);
	refBuilder->get_widget("lblClientesConectados", this->clientesConectados);
	refBuilder->get_widget("lblCarpetasActivas", this->carpetasActivas);
	
	// Botones
	refBuilder->get_widget("btnIniciar", this->botonIniciar);
	refBuilder->get_widget("btnConfiguracion", this->botonConfiguracion);
	refBuilder->get_widget("btnSalir", this->botonSalir);
	
	// Menu
	refBuilder->get_widget("mi_configuracion", this->menuConfiguracion);
	refBuilder->get_widget("mi_salir", this->menuSalir);
	refBuilder->get_widget("mi_admUsers", this->menuAdminUsers);
	refBuilder->get_widget("mi_estadisticas", this->menuEstadisticas);
	refBuilder->get_widget("mi_log", this->menuLog);
	refBuilder->get_widget("mi_manual", this->menuManualUsuario);
	refBuilder->get_widget("mi_ayuda", this->menuAyuda);

	// Acciones
	// Acciones -> Bontones
	this->botonIniciar->signal_clicked().connect(sigc::mem_fun(*this, &MenuPrincipal::on_buttonIniciar_clicked)); 
	this->botonConfiguracion->signal_clicked().connect(sigc::mem_fun(*this, &MenuPrincipal::on_buttonConfiguracion_clicked)); 
	this->botonSalir->signal_clicked().connect(sigc::mem_fun(*this, &MenuPrincipal::on_buttonSalir_clicked));
	
	// Acciones -> Menu

	this->menuConfiguracion->signal_activate().connect(sigc::mem_fun(*this, &MenuPrincipal::on_menuConfiguracion_activate));
	this->menuSalir->signal_activate().connect(sigc::mem_fun(*this, &MenuPrincipal::on_menuSalir_activate));
	this->menuAdminUsers->signal_activate().connect(sigc::mem_fun(*this, &MenuPrincipal::on_menuAdminUsers_activate));
	this->menuEstadisticas->signal_activate().connect(sigc::mem_fun(*this, &MenuPrincipal::on_menuEstadisticas_activate));
	this->menuLog->signal_activate().connect(sigc::mem_fun(*this, &MenuPrincipal::on_menuLog_activate));
	this->menuManualUsuario->signal_activate().connect(sigc::mem_fun(*this, &MenuPrincipal::on_menuManualUsuario_activate));
	this->menuAyuda->signal_activate().connect(sigc::mem_fun(*this, &MenuPrincipal::on_menuAyuda_activate));

	
	main->show_all_children();

}


void MenuPrincipal::on_buttonIniciar_clicked() {
	// Caso en que el servidor se encuentra detenido
	if(!this->servidor->estaActivo()) {
		// Actualizamos interfaz
		this->botonIniciar->set_sensitive(false);
		this->botonConfiguracion->set_sensitive(false);
		this->menuConfiguracion->set_sensitive(false);
		this->estado->set_text("Conectando");

		// Iniciamos servidor
		if(!this->servidor->iniciar(this->serverConfig->obtenerPuerto())) {
			// Falló la conexión, volvemos al estado anterior de la interfaz
			this->estado->set_text("Desconectado");
			this->botonIniciar->set_sensitive(true);
			this->botonConfiguracion->set_sensitive(true);
			this->menuConfiguracion->set_sensitive(true);

			// Enviamos a log
			std::cout << "ERROR: No ha sido posible iniciar el servidor." 
				<< std::endl;

			return;
		}
		
		// Actualizamos interfaz
		this->estado->set_text("Conectado");
		this->botonIniciar->set_label("Detener");
		this->botonIniciar->set_sensitive(true);

		// Falta actualizar la cantidad de usuarios conectados y las carpetas activas

		// DEBUG
		std::cout << "Inicio server"<< std::endl;
		// END DEBUG
		
		return;
	} 
	// Caso en que el servidor se encuentra activo y ejecutándose
	else {
		// Actualizamos interfaz
		this->botonIniciar->set_sensitive(false);
		this->estado->set_text("Deteniéndose");

		// Damos orden de detener servidor
		this->servidor->detener();
		this->servidor->join();

		// Actualizamos interfaz
		this->botonIniciar->set_label("Iniciar");
		this->estado->set_text("Desconectado");
		this->botonIniciar->set_sensitive(true);
		this->botonConfiguracion->set_sensitive(true);
		this->menuConfiguracion->set_sensitive(true);

		// DEBUG
		std::cout << "Server detenido"<< std::endl;
		// END DEBUG

		return;
	}
}

void MenuPrincipal::on_buttonSalir_clicked() {
	// Damos orden de detener servidor si se encuentra activo
	if(this->servidor->estaActivo()) {
		this->servidor->detener();
		this->servidor->join();

		// DEBUG
		std::cout << "Server detenido"<< std::endl;
		// END DEBUG
	}

	// Cerramos ventana
	Gtk::Main::quit();
}

void MenuPrincipal::on_buttonConfiguracion_clicked() {
	this->main->set_sensitive(false);
	IConfiguracion ventanaConfiguracion(this->serverConfig, 
		this->servidor->estaActivo());
	ventanaConfiguracion.correr();
	this->main->set_sensitive(true);
}

// Acciones del menu
void MenuPrincipal::on_menuConfiguracion_activate() {
	this->main->set_sensitive(false);
	IConfiguracion ventanaConfiguracion(this->serverConfig, 
		this->servidor->estaActivo());
	ventanaConfiguracion.correr();
	this->main->set_sensitive(true);
}


void MenuPrincipal::on_menuAdminUsers_activate(){} 
void MenuPrincipal::on_menuEstadisticas_activate(){}

void MenuPrincipal::on_menuLog_activate(){

}


void MenuPrincipal::on_menuManualUsuario_activate(){}
void MenuPrincipal::on_menuAyuda_activate(){}

void MenuPrincipal::on_menuSalir_activate() {
	// Damos orden de detener servidor si se encuentra activo
	if(this->servidor->estaActivo()) {
		this->servidor->detener();
		this->servidor->join();

		// DEBUG
		std::cout << "Server detenido"<< std::endl;
		// END DEBUG
	}

	// Cerramos ventana
	Gtk::Main::quit();
}

void MenuPrincipal::correr(){
	this->main->set_sensitive(true);
	Gtk::Main::run(*main);
}

MenuPrincipal::~MenuPrincipal() { }
