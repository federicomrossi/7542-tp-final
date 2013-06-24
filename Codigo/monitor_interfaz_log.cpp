#include <iostream>
#include <string>
#include "monitor_interfaz_log.h"

MenuLog::MenuLog(Monitor *monitor) : monitor(monitor) {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

	this->monitor->getReceptor()->stop();
	this->encendido = true;

	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/monitor_log.glade");
	refBuilder->get_widget("main", this->main); // linkeo el form

	// Etiquetas	
	
	// Botones
	
	refBuilder->get_widget("btn_limpiar", this->botonLimpiar);
	refBuilder->get_widget("btn_volver", this->botonVolver);
	refBuilder->get_widget("hojaLog", this->hojaLog);
	refBuilder->get_widget("btn_actualizar", this->botonActualizar);
	
	

	// Acciones
	// Acciones -> Bontones
	 
	this->botonLimpiar->signal_clicked().connect(sigc::mem_fun(*this, &MenuLog::on_buttonLimpiar_clicked)); 
	this->botonVolver->signal_clicked().connect(sigc::mem_fun(*this, &MenuLog::on_buttonVolver_clicked));
	this->botonActualizar->signal_clicked().connect(sigc::mem_fun(*this, &MenuLog::on_buttonActualizar_clicked));
	this->buffer = this->hojaLog->get_buffer();
	main->show_all_children();

	

}

// Se limpia todo el textView para vision mas clara 
void MenuLog::on_buttonLimpiar_clicked() {
 
	string unString = "";
	this->buffer->set_text(unString);

	
}

void MenuLog::on_buttonActualizar_clicked() {
	string* unString;
	unString = this->monitor->getBufferLog();
	this->buffer->insert_at_cursor(*unString);
	delete(unString);

}

void MenuLog::run() {

	/*while (this->encendido) {
		string msg;
		msg.append(M_SERVER_GET_LOG);
		this->monitor->getReceptor()->enviarMensaje(msg);
		msg.clear();
		this->monitor->getReceptor()->recibirMensaje(msg);
	
		sleep(this->velocidad);		


	*/
}


// Acciones del menu
void MenuLog::on_buttonVolver_clicked() {
	this->main->hide();
}


void MenuLog::correr(){
	this->main->set_sensitive(true);
	Gtk::Main::run(*main);
	this->main->hide();

}

MenuLog::~MenuLog() { 
}
