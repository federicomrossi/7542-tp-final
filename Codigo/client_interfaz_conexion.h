//
//  client_interfaz_conexion.h
//  CLASE CONEXION
//  


#ifndef CONEXION_H_
#define CONEXION_H_


#include "gtkmm.h"
#include "client_cliente.h"
#include "common_thread.h"
#include "client_interfaz_configuracion.h"



class Conexion : public Gtk::Window , Thread {
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



	//Glib::RefPtr<Gtk::StatusIcon> icono;		// IconTray de programa
	//GtkStatusIcon *icono;


	// Atributos del modelo
	Cliente *cliente;					// Cliente a través del cual se conecta
	Configuracion *clienteConfig;
	int estadoConexion;

public:

	// Constructor
	Conexion(Cliente *cliente, Configuracion* clienteConfig);

	// Destructor
	virtual ~Conexion();

	// Inicia la ejecución de la ventana
	void correr();

protected:

	void on_buttonConectar_clicked();
	void on_buttonSalir_clicked();
	void on_menuPref_activate();
	void on_menuSalir_activate();
	void run();

};

#endif /* CONEXION_H_ */
