#include <iostream>
#include <string>
#include "monitor_interfaz_principal.h"
#include "monitor_interfaz_configuracion.h"
#include "monitor_interfaz_usuarios.h"
#include "monitor_interfaz_log.h"
#include "common_convertir.h"



MenuPrincipal::MenuPrincipal(Monitor *monitor, Configuracion *config) : monitor(monitor), serverConfig(config) {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

	
	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/monitor_principal.glade");
	refBuilder->get_widget("main", this->main); // linkeo el form

	// Etiquetas	
	refBuilder->get_widget("lblEstado", this->estado);
	refBuilder->get_widget("lblClientesConectados", this->clientesConectados);
	refBuilder->get_widget("lblCarpetasActivas", this->carpetasActivas);
	
	// Botones
	
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


void MenuPrincipal::on_buttonSalir_clicked() {
	// Damos orden de detener servidor 
	
		std::cout << "Monitoreo detenido"<< std::endl;
		// END DEBUG
	

	// Cerramos ventana
	Gtk::Main::quit();
}

void MenuPrincipal::on_buttonConfiguracion_clicked() {
	this->main->set_sensitive(false);
	IConfiguracion ventanaConfiguracion(this->serverConfig,1);
	ventanaConfiguracion.correr();
	this->main->set_sensitive(true);
}

// Acciones del menu
void MenuPrincipal::on_menuConfiguracion_activate() {
	this->main->set_sensitive(false);
	IConfiguracion ventanaConfiguracion(this->serverConfig,this->monitor->getEstadoConexion());
	ventanaConfiguracion.correr();
	this->main->set_sensitive(true);
}


void MenuPrincipal::on_menuAdminUsers_activate() {
	this->main->set_sensitive(false);
	MenuUsuarios ventanaUsuarios(this->monitor);
	ventanaUsuarios.correr();
	this->main->set_sensitive(true);
} 


void MenuPrincipal::on_menuEstadisticas_activate(){}


void MenuPrincipal::on_menuLog_activate() {
	this->main->set_sensitive(false);
	MenuLog ventanaLog(this->monitor);
	ventanaLog.correr();
	this->main->set_sensitive(true);
}


void MenuPrincipal::on_menuManualUsuario_activate(){}
void MenuPrincipal::on_menuAyuda_activate(){}

void MenuPrincipal::on_menuSalir_activate() {
		// DEBUG
		std::cout << "Monitoreo detenido"<< std::endl;
		// END DEBUG
	// Cerramos ventana
	Gtk::Main::quit();
}

void MenuPrincipal::correr(){
	this->main->set_sensitive(true);
	this->start();
	Gtk::Main::run(*main);


}

void MenuPrincipal::run() {
	this->monitor->actualizarValores();
	while(monitor->getEstadoConexion()) { 
		this->monitor->actualizarValores();
		this->estado->set_text("Conectado");
		this->clientesConectados->set_text(monitor->getClientesConectados());
		this->carpetasActivas->set_text(monitor->getCarpetasActivas());
		// HAY QUE HACER VARIABLE EL INTERVALO DE ACTUALIZACION DE LA INTERFAZ?
		this->sleep(2);  
	}
	this->estado->set_text("Desconectado");
	this->clientesConectados->set_text("-");
	this->carpetasActivas->set_text("-");

}

MenuPrincipal::~MenuPrincipal() { }
