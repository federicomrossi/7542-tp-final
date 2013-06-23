#include <iostream>
#include <string>
#include "monitor_interfaz_principal.h"
#include "monitor_interfaz_configuracion.h"
#include "monitor_interfaz_usuarios.h"
#include "monitor_interfaz_log.h"
#include "monitor_interfaz_estadisticas.h"
#include "common_convertir.h"
#include "monitor_interfaz_formUsuario.h"




FormUsuario::FormUsuario(Monitor *monitor) {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

	this->monitor = monitor;
	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/monitor_formUsuario.glade");
	refBuilder->get_widget("main", this->main); // linkeo el form

	// Botones
	
	refBuilder->get_widget("btn_guardar", this->botonGuardar);
	refBuilder->get_widget("btn_cancelar", this->botonCancelar);
	

	// Acciones
	// Acciones -> Bontones
	 
	this->botonGuardar->signal_clicked().connect(sigc::mem_fun(*this, &FormUsuario::on_buttonGuardar_clicked)); 
	this->botonCancelar->signal_clicked().connect(sigc::mem_fun(*this, &FormUsuario::on_buttonCancelar_clicked));
	
	// Acciones -> Menu
	
	main->show_all_children();

}


void FormUsuario::on_buttonGuardar_clicked() {
	// Damos orden de detener servidor 
	
		std::cout << "ME guardo los valores para poder avisarle al server"<< std::endl;
		// END DEBUG
	

	// Cerramos ventana
	this->main->hide();
}



// Acciones del menu



void FormUsuario::on_buttonCancelar_clicked() {

	this->main->hide();
}

void FormUsuario::correr(){
	this->main->set_sensitive(true);
	Gtk::Main::run(*main);

}

FormUsuario::~FormUsuario() { }
