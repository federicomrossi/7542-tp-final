#include <iostream>
#include <string>
#include "monitor_interfaz_principal.h"
#include "common_lista.h"
#include "common_parser.h"
#include "monitor_interfaz_usuarios.h"
#include "common_protocolo.h"
#include "common_convertir.h"
#include "common_hash.h"
#include "monitor_interfaz_modificarUsuario.h"




ModificarUsuario::ModificarUsuario(Monitor *monitor, string aModificar) {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

	this->monitor = monitor;
	this->modificar = aModificar;

	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/monitor_formUsuarioModificacion.glade");
	refBuilder->get_widget("main", this->main); // linkeo el form

	
	refBuilder->get_widget("lbl_error", this->labelError);	
	refBuilder->get_widget("btn_guardar", this->botonGuardar);
	refBuilder->get_widget("btn_cancelar", this->botonCancelar);
		
	refBuilder->get_widget("txt_contrasenia", this->passTxt);	
	refBuilder->get_widget("txt_usuario",  this->usuarioTxt);

	// Acciones
	// Acciones -> Bontones
	 
	this->botonGuardar->signal_clicked().connect(sigc::mem_fun(*this, &ModificarUsuario::on_buttonGuardar_clicked)); 
	this->botonCancelar->signal_clicked().connect(sigc::mem_fun(*this, &ModificarUsuario::on_buttonCancelar_clicked));
	
	this->usuarioTxt->set_text(modificar);
	this->usuarioTxt->set_sensitive(false);


	main->show_all_children();

}


void ModificarUsuario::on_buttonGuardar_clicked() {
	
	string msg; 
	string res;
	string user;
	string pass;

	pass = this->passTxt->get_text();
	user = this->usuarioTxt->get_text();

	
	this->labelError->set_visible(false);
	
	
	if ((pass.compare("") == 0)) {

		this->labelError->set_visible(false);
		msg.clear();
		msg = "contraseña inválida";
		this->labelError->set_text(msg);
		this->labelError->set_visible(true);
		return;
		
	}

	this->labelError->set_visible(false);
	
	res.append(M_SERVER_MODIFY_USER_REQUEST);
	res.append(" ");
	
	res.append(user);
	res.append(COMMON_DELIMITER);
	res.append(Hash::funcionDeHash(pass));
	
	this->monitor->getReceptor()->enviarMensaje(res);
	this->main->hide();

}





void ModificarUsuario::on_buttonCancelar_clicked() {

	this->main->hide();
}

void ModificarUsuario::correr(){
	this->main->set_sensitive(true);
	Gtk::Main::run(*main);

}

ModificarUsuario::~ModificarUsuario() { }
