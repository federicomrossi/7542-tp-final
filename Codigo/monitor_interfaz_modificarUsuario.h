//
//  monitor_interfaz_modificarUsuario.h
//  CLASE MODIFICARUSUARIO
//  


#ifndef MODIFICARUSUARIO_H_
#define MODIFICARUSUARIO_H_


#include "gtkmm.h"

#include "monitor_monitor.h"




class ModificarUsuario {
private:

	// Atributos de la interfaz
	Gtk::Window *main;				 // 	
	
	Gtk::Button *botonGuardar;
	Gtk::Button *botonCancelar;
	Gtk::Label  *labelError;

	Gtk::Entry  *usuarioTxt;		// Textbox de nombre de usuario
	Gtk::Entry  *passTxt;		// Textbox de la contraseña de usuario

	Monitor *monitor;
	string modificar;


	
public:

	// Constructor
	ModificarUsuario(Monitor *monitor, string aModificar);

	// Destructor
	virtual ~ModificarUsuario();

	// Inicia la ejecución de la ventana
	void correr();

protected:
	// Acciones de botones
	
	void on_buttonGuardar_clicked();
	void on_buttonCancelar_clicked();

};

#endif /* FORMUSUARIO_H_ */
