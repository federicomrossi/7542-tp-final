//
//  monitor_interfaz_eliminarUsuario.h
//  CLASE FORMCONFIRMACION
//  


#ifndef FORMCONFIRMACION_H_
#define FORMCONFIRMACION_H_


#include "gtkmm.h"
#include <string>
#include "monitor_monitor.h"




class FormConfirmacion {
private:

	// Atributos de la interfaz
	Gtk::Window *main;				
	
	Gtk::Button *botonAceptar;
	Gtk::Button *botonCancelar;
	Gtk::Label  *mensaje;
	// Atributos del modelo
	Monitor *monitor;
	string borrar;
	int seleccion;
	
public:

	// Constructor
	FormConfirmacion(Monitor *monitor, string borrar);

	// Destructor
	virtual ~FormConfirmacion();

	// Inicia la ejecución de la ventana
	int correr();

protected:
	// Acciones de botones
	
	void on_buttonAceptar_clicked();
	void on_buttonCancelar_clicked();

};

#endif /* FORMUSUARIO_H_ */
