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

	
	refBuilder->get_widget("lbl_error", this->labelError);	
	refBuilder->get_widget("btn_guardar", this->botonGuardar);
	refBuilder->get_widget("btn_cancelar", this->botonCancelar);
	refBuilder->get_widget("txt_contrasenia", this->passTextBox);	
	refBuilder->get_widget("txt_usuario",  this->usuarioTextBox);

	// Acciones
	// Acciones -> Bontones
	 
	this->botonGuardar->signal_clicked().connect(sigc::mem_fun(*this, &FormUsuario::on_buttonGuardar_clicked)); 
	this->botonCancelar->signal_clicked().connect(sigc::mem_fun(*this, &FormUsuario::on_buttonCancelar_clicked));
	
	// Acciones -> Menu
	
	main->show_all_children();

}


void FormUsuario::on_buttonGuardar_clicked() {
	
	string msg; 
	string res;
	string user = this->usuarioTextBox->get_text();
	string pass = this->passTextBox->get_text();
	
	// recorro la lista de usuarios y comparo si hay alguno igual a lo guardado en el textbox
	// si hay uno igual..cambio el estado de una label
	
	this->labelError->set_visible(false);
	int flag = 0;
	
	if ((user.compare("") == 0) ||  (user.compare(" ") == 0) || (pass.compare("") == 0)) {

		this->labelError->set_visible(false);
		msg.clear();
		msg = "Contenido Inválido";
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

	res.append(user);
	res.append(pass);
	std::cout<<res << std::endl;
	this->main->hide();
	}
	
	
	// sino... 
	//mando mensaje al sever
	//  y retorno el string del text box para que actualice la lsita del modelo







void FormUsuario::on_buttonCancelar_clicked() {

	this->main->hide();
}

void FormUsuario::correr(){
	this->main->set_sensitive(true);
	Gtk::Main::run(*main);

}

FormUsuario::~FormUsuario() { }
