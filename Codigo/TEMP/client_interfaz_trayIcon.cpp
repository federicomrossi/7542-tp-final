#include <iostream>
#include <string>
#include "client_configuracion.h"
#include "client_interfaz_trayIcon.h"



TrayIcon::TrayIcon(Cliente *cliente) : cliente(cliente) {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
	
	
	
	// Cargamos elementos
	//refBuilder->add_from_file("./interfaz/trayIcon.glade");
	this->icono = gtk_status_icon_new_from_icon_name("statusicon");
	gtk_status_icon_set_from_file(this->icono,"logo_au.png");
	

}


void TrayIcon::correr(){
	Gtk::Main::run();
}

TrayIcon::~TrayIcon() { }

