//
//  client_interfaz_conexion.h
//  CLASE CONEXION
//  


#ifndef CONEXION_H_
#define CONEXION_H_


#include "gtkmm.h"
#include "client_cliente.h"



class Conexion : public Gtk::Window {
private:

	// Atributos de la interfaz
	Gtk::Window* main;					// Ventana
	Gtk::Label	*lblError;				// Etiqueta de error
	Gtk::Button *botonConectar;			// Botón Conectar
	Gtk::Button *botonSalir;			// Botón Salir
	Gtk::Entry 	*usuarioTextBox;		// Textbox de nombre de usuario
	Gtk::Entry  *passTextBox;			// Textbox de la contraseña de usuario
	//Glib::RefPtr<Gtk::StatusIcon> icono;		// IconTray de programa
	GtkStatusIcon *icono;
	// Atributos del modelo
	Cliente *cliente;					// Cliente a través del cual se conecta

public:

	// Constructor
	Conexion(Cliente *cliente);

	// Destructor
	virtual ~Conexion();

	// Inicia la ejecución de la ventana
	void correr();

protected:

	void on_buttonConectar_clicked();
	void on_buttonSalir_clicked();
};

#endif /* CONEXION_H_ */
