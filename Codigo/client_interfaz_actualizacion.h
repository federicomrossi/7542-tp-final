//  
//  client_interfaz_actualizacion.h
//  CLASE INTERFAZ DE ACTUALIZACION
//  


#ifndef IACTUALIZACION_H_
#define IACTUALIZACION_H_


#include "gtkmm.h"
#include "common_thread.h"
#include "client_cliente.h"



class IActualizacion : public Gtk::Window, public Thread {
private:

	// Atributos de la interfaz
	Gtk::Window* main;			// Ventana
	Cliente *cliente;				// Cliente que se actualiza	
	
public:

	// Constructor
	IActualizacion (Cliente *cliente); 

	// Destructor
	virtual ~IActualizacion();

	// Define tareas a ejecutar en el hilo.
	virtual void run();
	void detener();
	
	void correr();
};

#endif
