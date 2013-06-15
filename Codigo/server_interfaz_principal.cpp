#include <iostream>
#include <string>
#include "server_interfaz_principal.h"



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
	// Iniciamos servidor -> falta validar que pasa si arranca mal el servidor!!!!

		if(this->estado->get_text() == "Desconectado") {
			this->servidor->iniciar(this->serverConfig->obtenerPuerto());
			std::cout << "Inicio server"<< std::endl;
			this->estado->set_text("Conectado   ");
			this->botonIniciar->set_label("Detener");

			// Falta actualizar la cantidad de usuarios conectados y las carpetas activas
			return;
		} 

		if ((this->estado->get_text()) == "Conectado   ") {
			this->servidor->detener();
			this->estado->set_text("Desconectado");
			this->botonIniciar->set_label("Iniciar");

			return;
		}
}

void MenuPrincipal::on_buttonSalir_clicked() {
	Gtk::Main::quit();

}



void MenuPrincipal::on_buttonConfiguracion_clicked() {

	
}

// Acciones del menu
void MenuPrincipal::on_menuConfiguracion_activate(){}

void MenuPrincipal::on_menuAdminUsers_activate(){} 
void MenuPrincipal::on_menuEstadisticas_activate(){}
void MenuPrincipal::on_menuLog_activate(){}
void MenuPrincipal::on_menuManualUsuario_activate(){}
void MenuPrincipal::on_menuAyuda_activate(){}

void MenuPrincipal::on_menuSalir_activate() {
	Gtk::Main::quit();
}

void MenuPrincipal::correr(){
	Gtk::Main::run(*main);
}

MenuPrincipal::~MenuPrincipal() { }
