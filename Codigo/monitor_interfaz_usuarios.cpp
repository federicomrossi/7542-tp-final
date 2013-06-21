#include <iostream>
#include <string>
#include "monitor_interfaz_usuarios.h"
#include "common_convertir.h"



MenuUsuarios::MenuUsuarios(Monitor *monitor) : monitor(monitor) {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

	
	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/monitor_adminUsuarios.glade");
	refBuilder->get_widget("main", this->main); // linkeo el form

	
	// Botones
	
	refBuilder->get_widget("btn_nuevoUsuario", this->botonNuevo);
	refBuilder->get_widget("btn_eliminarUsuario", this->botonEliminar);
	refBuilder->get_widget("btn_modificarUsuario", this->botonModificar);
	refBuilder->get_widget("btn_volver", this->botonVolver);
	

	// Acciones
	// Acciones -> Bontones
	 
	this->botonNuevo->signal_clicked().connect(sigc::mem_fun(*this, &MenuUsuarios::on_buttonNuevo_clicked)); 
	this->botonEliminar->signal_clicked().connect(sigc::mem_fun(*this, &MenuUsuarios::on_buttonEliminar_clicked));
	this->botonModificar->signal_clicked().connect(sigc::mem_fun(*this, &MenuUsuarios::on_buttonModificar_clicked)); 
	this->botonVolver->signal_clicked().connect(sigc::mem_fun(*this, &MenuUsuarios::on_buttonVolver_clicked)); 
	
	main->show_all_children();

}


void MenuUsuarios::on_buttonNuevo_clicked() {
	 
	
}

void MenuUsuarios::on_buttonEliminar_clicked() {
	/*this->main->set_sensitive(false);
	IConfiguracion ventanaConfiguracion(this->serverConfig,1);
	ventanaConfiguracion.correr();
	this->main->set_sensitive(true);
	*/
}


void MenuUsuarios::on_buttonModificar_clicked() {
	/*this->main->set_sensitive(false);
	IConfiguracion ventanaConfiguracion(this->serverConfig,this->monitor->getEstadoConexion());
	ventanaConfiguracion.correr();
	this->main->set_sensitive(true);
	*/
}

void MenuUsuarios::on_buttonVolver_clicked() {

	this->main->hide();

}
void MenuUsuarios::correr() { 
	this->main->set_sensitive(true);
	Gtk::Main::run(*main);

}

MenuUsuarios::~MenuUsuarios() { }
