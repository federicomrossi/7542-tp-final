/*
 * conexion.cpp
 *
 *  Created on: 27/05/2013
 *      Author: fiona
 */
#include "client_mainCliente.h"
#include "client_conexion.h"
#include <iostream>
#include <string>


using namespace std;

conexion::conexion() {
		Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
		refBuilder->add_from_file("./conexion.glade");

		refBuilder->get_widget("conexion", main); // linkeo el form

		refBuilder->get_widget("usuarioTxt", usuarioTextBox);
		refBuilder->get_widget("passTxt", passTextBox);

		refBuilder->get_widget("conectar", botonConectar);
		botonConectar->signal_clicked().connect(sigc::mem_fun(*this, &conexion::on_buttonConectar_clicked));
		refBuilder->get_widget("Salir", botonSalir);
		botonSalir->signal_clicked().connect(sigc::mem_fun(*this, &conexion::on_buttonSalir_clicked));
		main->show_all_children();
	}


void conexion::on_buttonConectar_clicked(){
	
	string usuario = usuarioTextBox->get_text();
	string pass = passTextBox->get_text();
	MainCliente m;
	m.conectar(usuario, pass); 
	
	
}

void conexion::on_buttonSalir_clicked(){
	Gtk::Main::quit();

}


void conexion::correr(){
	Gtk::Main::run(*main);

}

conexion::~conexion() {
	// TODO Auto-generated destructor stub
}

