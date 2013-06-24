#include <iostream>
#include <string>
#include "common_lista.h"
#include "common_parser.h"
#include "monitor_interfaz_log.h"
#include "common_convertir.h"

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
	refBuilder->get_widget("btn_iniciar", this->botonIniciar);
	
	

	// Acciones
	// Acciones -> Bontones
	 
	this->botonLimpiar->signal_clicked().connect(sigc::mem_fun(*this, &MenuLog::on_buttonLimpiar_clicked)); 
	this->botonVolver->signal_clicked().connect(sigc::mem_fun(*this, &MenuLog::on_buttonVolver_clicked));
	this->botonIniciar->signal_clicked().connect(sigc::mem_fun(*this, &MenuLog::on_buttonIniciar_clicked));
	this->buffer = this->hojaLog->get_buffer();
	this->ajuste = this->boton
	main->show_all_children();

	

}

// Se limpia todo el textView para vision mas clara 
void MenuLog::on_buttonLimpiar_clicked() {
 
	string unString = "";
	this->buffer->set_text(unString);

	
}

void MenuLog::on_buttonIniciar_clicked() {
	
		this->encendido = true;
		this->start();
		
		this->botonIniciar->set_sensitive(false);
	}


	
	


void MenuLog::run() {
	this->velocidad = 3;

	string* muestro = new string();
	
	Lista <std::string> aux;
 	string instruccion; 
 	string args;

	while (this->encendido && this->isActive()) {
		
		string msg;
		msg.append(M_SERVER_GET_LOG);
		this->monitor->getReceptor()->enviarMensaje(msg);
		this->monitor->getReceptor()->recibirMensaje(msg);
	 	
	 	Parser::parserInstruccion(msg, instruccion, args);
		Parser::dividirCadena(args, &aux, COMMON_DELIMITER[0]);
		std::cout << aux[0]<<std::endl;
		*muestro += aux[0];


		this->buffer->insert_at_cursor(*muestro);
		muestro->clear();

	
		sleep(this->velocidad);		
	}

	delete(muestro);
}


// Acciones del menu
void MenuLog::on_buttonVolver_clicked() {
	this->encendido = false;
	this->join();
	this->main->hide();
	this->monitor->getReceptor()->start();
}


void MenuLog::correr(){
	this->main->set_sensitive(true);
	Gtk::Main::run(*main);
	this->main->hide();

}

MenuLog::~MenuLog() { 
}
