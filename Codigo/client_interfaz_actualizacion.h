//
//  client_interfaz_actualizacion.h
//  CLASE ACTUALIZACION
//  


#ifndef INTERFAZ_ACTUALIZACION_H_
#define INTERFAZ_ACTUALIZACION_H_


#include "gtkmm.h"



class Actualizacion : public Gtk::Window {
private:

	// Atributos de la interfaz
	Gtk::Window *ventana;						// Ventana
	Gtk::Label	*lblActividad;					// Etiqueta de actividad
	Gtk::Label	*lblDetalles;					// Link de detalles
	Gtk::ProgressBar *prgbarCompletado;			// Barra de progreso

public:

	// Constructor
	Actualizacion();

	// Destructor
	virtual ~Actualizacion();

	// Inicia la ejecución de la ventana
	void correr();

protected:

};

#endif
