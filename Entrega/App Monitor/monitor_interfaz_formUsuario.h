//
//  monitor_interfaz_formUsuario.h
//  CLASE FORMUSUARIO
//  


#ifndef FORMUSUARIO_H_
#define FORMUSUARIO_H_


#include "gtkmm.h"

#include "monitor_monitor.h"




class FormUsuario {
private:

	// Atributos de la interfaz
	Gtk::Window *main;					
	
	Gtk::Button *botonGuardar;
	Gtk::Button *botonCancelar;
	Gtk::Label  *labelError;


	Gtk::Entry  *usuarioTextBox;		// Textbox de nombre de usuario
	Gtk::Entry  *passTextBox;			// Textbox de la contraseña de usuario

	Monitor *monitor;

	
public:

	// Constructor
	FormUsuario(Monitor *monitorm);

	// Destructor
	virtual ~FormUsuario();

	// Inicia la ejecución de la ventana
	void correr();

protected:
	// Acciones de botones
	
	void on_buttonGuardar_clicked();
	void on_buttonCancelar_clicked();

};

#endif /* FORMUSUARIO_H_ */
