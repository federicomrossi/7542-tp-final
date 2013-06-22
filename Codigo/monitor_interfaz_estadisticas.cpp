#include <iostream>
#include <string>
#include "monitor_interfaz_estadisticas.h"



MenuEstadisticas::MenuEstadisticas(Monitor *monitor) { // pero hay q setearlo.. osea hay q pasar el pntero y mandarlo un atributo?
	// Cargamos la ventana pr
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

	
	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/monitor_estadisticas.glade");
	refBuilder->get_widget("main", this->main); // linkeo el form


	main->show_all_children();
	this->monitor = monitor;
	this->c.monitor = monitor;
}

void MenuEstadisticas::correr() {
	//la dejamos solo porq sabemos q se dibuja
	
	//aca agregamos la coleccion de vistas a la ventana
	c.show_all();
	main->add(c);

	c.correr();
	Gtk::Main::run(*main);
	c.detener();
} 

MenuEstadisticas::~MenuEstadisticas() { }
