//
//  monitor_interfaz_Log.h
//  CLASE MENULOG
//  


#ifndef MENULOG_H_
#define MENULOG_H_


#include "gtkmm.h"
#include "monitor_monitor.h"
#include "monitor_configuracion.h"



class MenuLog : public Gtk::Window {
private:

	// Atributos de la interfaz
	Gtk::Window *main;				 // Ventana Conexion
	Gtk::Button *botonLimpiar;		 // Limpia la interfaz
	Gtk::Button *botonVolver;
	Gtk::Button *botonActualizar;
	Gtk::TextView *hojaLog;
	Glib::RefPtr<Gtk::TextBuffer> buffer;
	
	
	// Atributos del modelo

	Monitor *monitor;
		
public:

	// Constructor
	MenuLog(Monitor *monitor);

	// Destructor
	virtual ~MenuLog();

	// Inicia la ejecución de la ventana
	void correr();

protected:
	// Acciones de botones
	
	void on_buttonLimpiar_clicked();
	void on_buttonVolver_clicked();
	void on_buttonActualizar_clicked();
	
};

#endif /* MENULOG_H_ */
