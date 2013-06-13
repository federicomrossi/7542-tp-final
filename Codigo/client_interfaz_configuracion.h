//
//  client_interfaz_iconfiguracion.h
//  CLASE INTERFAZ DE CONFIGURACION
//  


#ifndef ICONFIGURACION_H_
#define ICONFIGURACION_H_


#include "client_configuracion.h"
#include "gtkmm.h"




class IConfiguracion : public Gtk::Window {
private:

	// Atributos de la interfaz
	Gtk::Window* main;					
					
	Gtk::Button *botonGuardar;			
	Gtk::Button *botonCancelar;			
	Gtk::Entry  *host;	
	Gtk::Entry  *puerto;
	Gtk::Entry  *directorio;	
	Gtk::Entry  *iPolling;

	Configuracion* config;		
	
	
public:

	// Constructor
	IConfiguracion(Configuracion *config); 

	// Destructor
	virtual ~IConfiguracion();

	// Inicia la ejecución de la ventana
	void correr();

protected:

	void on_buttonGuardar_clicked();
	void on_buttonCancelar_clicked();
};

#endif /* ICONFIGURACION_H_ */
