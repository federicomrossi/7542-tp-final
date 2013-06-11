//
//  client_interfaz_trayIcon.h
//  CLASE TRAYICON
//  


#ifndef TRAYICON_H_
#define TRAYICON_H_


#include "gtkmm.h"
#include "client_cliente.h"



class TrayIcon : public Gtk::Window {
private:

	// Atributos de la interfaz
	
	//Glib::RefPtr<Gtk::StatusIcon> icono;		// IconTray de programa
	GtkStatusIcon *icono;
	// Atributos del modelo
	Cliente *cliente;					// Cliente a través del cual se conecta

public:

	// Constructor
	TrayIcon(Cliente *cliente);

	// Destructor
	virtual ~TrayIcon();

	// Inicia la ejecución de la ventana
	void correr();

protected:

	
};

#endif /* TRAYICON_H_ */
