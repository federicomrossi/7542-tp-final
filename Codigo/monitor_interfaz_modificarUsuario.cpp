#include <iostream>
#include <string>
#include "monitor_interfaz_principal.h"
#include "common_lista.h"
#include "common_parser.h"
#include "monitor_interfaz_usuarios.h"
#include "common_protocolo.h"
#include "common_convertir.h"
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
	refBuilder->get_widget("txt_contraseniaActual", this->passActualTxt);	
	refBuilder->get_widget("txt_contraseniaNueva", this->passNuevaTxt);	
	refBuilder->get_widget("txt_usuario",  this->usuarioTxt);

	// Acciones
	// Acciones -> Bontones
	 
	this->botonGuardar->signal_clicked().connect(sigc::mem_fun(*this, &ModificarUsuario::on_buttonGuardar_clicked)); 
	this->botonCancelar->signal_clicked().connect(sigc::mem_fun(*this, &ModificarUsuario::on_buttonCancelar_clicked));
	
	this->usuarioTxt->set_text(modificar);


	string msg;
	msg.append(M_SERVER_MODIFY_USER_REQUEST);
	msg.append(" ");
	msg.append(aModificar);
	this->monitor->getReceptor()->enviarMensaje(msg);
	msg.clear();
	this->monitor->getReceptor()->recibirMensaje(msg);
	Lista <std::string> aux;
 	string instruccion; 
 	string args;
 	Parser::parserInstruccion(msg, instruccion, args);
	Parser::dividirCadena(args, &aux, COMMON_DELIMITER[0]);
	this->passActualTxt->set_text(aux[0]);
	this->passActualTxt->set_sensitive(false);
	this->passNuevaTxt->set_text(aux[0]);

	this->userOriginal = this->usuarioTxt->get_text();
	
	main->show_all_children();

}


void ModificarUsuario::on_buttonGuardar_clicked() {
	
	string msg; 
	string res;
	string user;
	string pass;

	pass = this->passNuevaTxt->get_text();
	user = this->usuarioTxt->get_text();

	this->monitor->usuarios.eliminar(userOriginal);


	
	
	this->labelError->set_visible(false);
	int flag = 0;
	
	if ((user.compare("") == 0) ||  (user.compare(" ") == 0) || (pass.compare("") == 0)) {

		this->labelError->set_visible(false);
		msg.clear();
		msg = "usuario/contraseña inválido";
		this->labelError->set_text(msg);
		this->labelError->set_visible(true);
		return;
		
	}

	this->labelError->set_visible(false);
	
	for (size_t i = 0; i < (this->monitor->usuarios.tamanio()); i++) {	
		if (user.compare(this->monitor->usuarios[i]) == 0) {
			msg = "El usuario ingresado ya existe";
			this->labelError->set_text(msg);
			this->labelError->set_visible(true);
			flag = 1;
			break;
		} 

	}
	if (flag == 1) return;

	res.append(M_SERVER_MODIFY_USER);
	res.append(" ");
	res.append(this->modificar);
	res.append(COMMON_DELIMITER);
	res.append(user);
	res.append(COMMON_DELIMITER);
	res.append(pass);
	this->monitor->usuarios.insertarUltimo(user);
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
