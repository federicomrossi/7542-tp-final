#include <iostream>

#include "monitor_interfaz_usuarios.h"
#include "common_protocolo.h"
#include "common_convertir.h"
#include "monitor_interfaz_eliminarUsuario.h"




FormConfirmacion::FormConfirmacion(Monitor *monitor, string borrar) {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

	this->monitor = monitor;
	this->borrar = borrar;

	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/monitor_confirmarEliminacion.glade");
	refBuilder->get_widget("main", this->main); // linkeo el form

	
	refBuilder->get_widget("lbl_msg", this->mensaje);	
	refBuilder->get_widget("btn_aceptar", this->botonAceptar);
	refBuilder->get_widget("btn_cancelar", this->botonCancelar);


	// Acciones
	// Acciones -> Bontones
	 
	this->botonAceptar->signal_clicked().connect(sigc::mem_fun(*this, &FormConfirmacion::on_buttonAceptar_clicked)); 
	this->botonCancelar->signal_clicked().connect(sigc::mem_fun(*this, &FormConfirmacion::on_buttonCancelar_clicked));
	
	// preparamos la ventana
	string lbl;
	this->mensaje->set_visible(false);
	lbl.append("Se eliminará al usuario: ");
	lbl.append(this->borrar);
	this->mensaje->set_text(lbl);
	this->mensaje->set_visible(true);
	
	main->show_all_children();

}

void FormConfirmacion::on_buttonAceptar_clicked() {
	string msg;
	msg.append(M_SERVER_DELETE_USER);
	msg.append(" ");
	msg.append(this->borrar);

	this->monitor->getReceptor()->enviarMensaje(msg);
	this->main->hide();
}
void FormConfirmacion::on_buttonCancelar_clicked() {

	this->main->hide();
}

void FormConfirmacion::correr(){
	this->main->set_sensitive(true);
	Gtk::Main::run(*main);

}

FormConfirmacion::~FormConfirmacion() { }
