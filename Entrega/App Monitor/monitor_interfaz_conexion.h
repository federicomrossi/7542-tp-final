//
//  monitor_interfaz_conexion.h
//  CLASE CONEXION
//  


#ifndef CONEXION_H_
#define CONEXION_H_


#include "gtkmm.h"
#include "monitor_receptorDatos.h"
#include "monitor_configuracion.h"
#include "monitor_interfaz_configuracion.h"



class Conexion : public Gtk::Window {
private:

	// Atributos de la interfaz
	Gtk::Window* main;				// Ventana Conexion
	
	Gtk::Label  *lblError;			// Etiqueta de error
	Gtk::Button *botonConectar;		// Botón Conectar
	Gtk::Button *botonSalir;		// Botón Salir
	Gtk::Entry  *usuarioTextBox;		// Textbox de nombre de usuario
	Gtk::Entry  *passTextBox;		// Textbox de la contraseña de usuario

	//Atributos del menu
	Gtk::ImageMenuItem *menuPref;
	Gtk::ImageMenuItem *menuSalir;


	// Atributos del modelo
	Receptor *receptor;					// Cliente a través del cual se conecta
	Configuracion *receptorConfig;
	int estadoConexion;
	int flagSalida;

public:

	// Constructor
	Conexion(Receptor *receptor, Configuracion* receptorConfig);

	// Destructor
	virtual ~Conexion();

	// Inicia la ejecución de la ventana
	int correr();

protected:

	void on_buttonConectar_clicked();
	void on_buttonSalir_clicked();
	void on_menuPref_activate();
	void on_menuSalir_activate();

};

#endif /* CONEXION_H_ */
