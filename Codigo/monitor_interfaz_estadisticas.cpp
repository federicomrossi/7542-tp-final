#include <iostream>
#include <string>
#include "monitor_interfaz_estadisticas.h"



MenuEstadisticas::MenuEstadisticas(Monitor *monitor) {
	
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

	
	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/monitor_estadisticas.glade");
	refBuilder->get_widget("main", this->main); // linkeo el form en blanco donde se muestra el grafico

	main->show_all_children();
	this->monitor = monitor;
	this->c.monitor = monitor;
}

void MenuEstadisticas::correr() {

	//cargamos drawing(graficador) a la ventana(main) se corre el hilo del graficador
	c.show_all();
	main->add(c);

	c.correr();
	Gtk::Main::run(*main);
	c.detener();
	main->hide();
} 

MenuEstadisticas::~MenuEstadisticas() { }
