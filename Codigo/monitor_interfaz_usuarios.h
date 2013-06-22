//
//  monitor_interfaz_usuarios.h
//  CLASE MENUUSUARIOS
//  


#ifndef MENUUSUARIOS_H_
#define MENUUSUARIOS_H_


#include "gtkmm.h"

#include "monitor_monitor.h"


class MenuUsuarios {
private:

	// Atributos de la interfaz
	Gtk::Window *main;				 // Ventana De ABM de usuarios
	
	
	Gtk::Button *botonNuevo;   
	Gtk::Button *botonEliminar;
	Gtk::Button *botonModificar;
	Gtk::Button *botonVolver;


protected:

  //Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    { add(m_col_name);}
  
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;

    
  };
  Gtk::Grid *grid;
  ModelColumns m_Columns;
  Gtk::TreeView tree;
  Glib::RefPtr<Gtk::ListStore> listaUsuarios;
  Glib::RefPtr<Gtk::TreeSelection>  seleccionado;
	

	// Atributos del modelo

	Monitor *monitor;
	
	
public:

	// Constructor
	MenuUsuarios(Monitor *monitor);

	// Destructor
	virtual ~MenuUsuarios();

	// Inicia la ejecución de la ventana
	void correr();

	void on_buttonNuevo_clicked();
	
	void on_buttonEliminar_clicked();

	void on_buttonModificar_clicked();

	void on_buttonVolver_clicked();
	void on_selection_changed(); 


};

#endif /* MENUUSUARIOS_H_ */
