#include <iostream>
#include <string>
#include "monitor_interfaz_usuarios.h"
#include "monitor_interfaz_formUsuario.h"
#include "monitor_interfaz_eliminarUsuario.h"
#include "monitor_interfaz_modificarUsuario.h"
#include "common_convertir.h"

#include <iostream>
#include <string>

#include "common_convertir.h"



MenuUsuarios::MenuUsuarios(Monitor *monitor) : monitor(monitor) {
	// Cargamos la ventana
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();


	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/monitor_adminUsuarios.glade");
	refBuilder->get_widget("main", this->main); // linkeo el form
	refBuilder->get_widget("grid1", this->grid);


	// Botones

	refBuilder->get_widget("btn_nuevoUsuario", this->botonNuevo);
	refBuilder->get_widget("btn_eliminarUsuario", this->botonEliminar);
	refBuilder->get_widget("btn_modificarUsuario", this->botonModificar);
	refBuilder->get_widget("btn_volver", this->botonVolver);

	this->grid->attach(this->tree, 0, 0, 1, 1);


	//Create the Tree model:

 	 this->listaUsuarios = Gtk::ListStore::create(m_Columns);
 	 this->tree.set_model(this->listaUsuarios);

 	// Actualizo la lista de usuarios existentes
 	this->monitor->getReceptor()->stop();
 	this->monitor->getUsuarios();
 	



 	// Cargo la lisa de usuarios a la lista que se muestra por pantalla.
  	//Fill the TreeView's model

  	 for (size_t i = 0; i < (this->monitor->usuarios.tamanio()); i++) {
  	 	Gtk::TreeModel::Row row = *(this->listaUsuarios->append());
  	 	row[m_Columns.m_col_name] =  this->monitor->usuarios[i];
  	 }

  	//Establacemos el titulo de a columna a mostrar
  	this->tree.append_column("Usuario             |", m_Columns.m_col_name); 
  	

	// Acciones
	// Acciones -> Bontones


    this->botonEliminar->set_sensitive( false );
    this->seleccionado = this->tree.get_selection();

	this->botonNuevo->signal_clicked().connect(sigc::mem_fun(*this, &MenuUsuarios::on_buttonNuevo_clicked)); 
	this->botonEliminar->signal_clicked().connect(sigc::mem_fun(*this, &MenuUsuarios::on_buttonEliminar_clicked));
	this->botonModificar->signal_clicked().connect(sigc::mem_fun(*this, &MenuUsuarios::on_buttonModificar_clicked)); 
	this->botonVolver->signal_clicked().connect(sigc::mem_fun(*this, &MenuUsuarios::on_buttonVolver_clicked)); 

	this->seleccionado->signal_changed().connect( sigc::mem_fun(*this, &MenuUsuarios::on_selection_changed) );


	main->show_all_children();

}


void MenuUsuarios::on_buttonNuevo_clicked() {

	this->main->set_sensitive(false);
	FormUsuario ventanaDeEdicion(this->monitor);
	ventanaDeEdicion.correr();
	this->main->set_sensitive(true);
	size_t pos = this->monitor->usuarios.tamanio();
	Gtk::TreeModel::Row row  = *(this->listaUsuarios->append());
	row[m_Columns.m_col_name] = this->monitor->usuarios[pos-1]; // ingreso el ultimo agregado a la vista

}


void MenuUsuarios::on_buttonEliminar_clicked() {
	
	this->main->set_sensitive(false);
	Gtk::TreeModel::iterator store_iter = this->seleccionado->get_selected();
	string aBorrar;
	if(store_iter) {

		Gtk::TreeModel::Row row = *store_iter;
  		
  		aBorrar = row[m_Columns.m_col_name];
  		
	}
	
	FormConfirmacion ventanaConfirmacion(this->monitor, aBorrar);
	ventanaConfirmacion.correr();
	this->listaUsuarios->erase(store_iter);    
	this->monitor->usuarios.eliminar(aBorrar);
	this->main->set_sensitive(true);


}


void MenuUsuarios::on_buttonModificar_clicked() {
	

	Gtk::TreeModel::iterator store_iter = this->seleccionado->get_selected();
	string aModificar;
	if(store_iter) {

		Gtk::TreeModel::Row row = *store_iter;
  		
  		aModificar = row[m_Columns.m_col_name];
  		
	}
	this->main->set_sensitive(false);

	ModificarUsuario ventanaModificacion(this->monitor, aModificar);
	ventanaModificacion.correr();
	this->main->set_sensitive(true);

	// Borramos e ingresamos al nuevo al modelo
	this->listaUsuarios->erase(store_iter);
	// ingreso el ultimo agregado a la vista
	size_t pos = this->monitor->usuarios.tamanio();
	Gtk::TreeModel::Row row  = *(this->listaUsuarios->append());
	row[m_Columns.m_col_name] = this->monitor->usuarios[pos-1]; 





}

void MenuUsuarios::on_buttonVolver_clicked() {
	this->monitor->getReceptor()->start();
	this->main->hide();
}

void MenuUsuarios::on_selection_changed() {

       this->botonEliminar->set_sensitive(
		this->seleccionado->count_selected_rows() > 0 );
}


void MenuUsuarios::correr() { 
	this->main->set_sensitive(true);
	Gtk::Main::run(*main);

}

MenuUsuarios::~MenuUsuarios() { }

