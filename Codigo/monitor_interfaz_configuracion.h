//
//  server_interfaz_configuracion.h
//  CLASE INTERFAZ DE CONFIGURACION
//  


#ifndef ICONFIGURACION_H_
#define ICONFIGURACION_H_


#include "monitor_configuracion.h"
#include "gtkmm.h"




class IConfiguracion : public Gtk::Window {
private:

	// Atributos de la interfaz
	Gtk::Window* main;					
					
	Gtk::Button *botonGuardar;			
	Gtk::Button *botonCancelar;			
	
	Gtk::Entry  *puerto;
	
	Configuracion* config;	
	int flag; // Indica si el servidor esta conectado	
	
	
public:

	// Constructor
	IConfiguracion(Configuracion *config, int flag); 

	// Destructor
	virtual ~IConfiguracion();

	// Inicia la ejecución de la ventana
	void correr();

protected:

	void on_buttonGuardar_clicked();
	void on_buttonCancelar_clicked();
};

#endif /* ICONFIGURACION_H_ */
