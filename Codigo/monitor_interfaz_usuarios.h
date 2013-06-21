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

};

#endif /* MENUUSUARIOS_H_ */
