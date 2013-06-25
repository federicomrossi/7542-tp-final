//
//  monitor_interfaz_principal.h
//  CLASE MENUPRINCIPAL
//  


#ifndef MENUPRINCIPAL_H_
#define MENUPRINCIPAL_H_


#include "gtkmm.h"
#include "common_thread.h"
#include "monitor_monitor.h"
#include "monitor_configuracion.h"



class MenuPrincipal : public Thread {
private:

	// Atributos de la interfaz
	Gtk::Window *main;				 // Ventana Conexion
	
	Gtk::Label  *estado;			 // Estado del servidor
	Gtk::Label  *clientesConectados;
	Gtk::Label	*carpetasActivas;
	
	
	Gtk::Button *botonConfiguracion; // Botón Salir
	Gtk::Button *botonSalir;
	
	//Atributos del menu
	//Monitor
	Gtk::ImageMenuItem *menuConfiguracion;
	Gtk::ImageMenuItem *menuSalir;
	//Opciones
	Gtk::ImageMenuItem *menuAdminUsers;
	Gtk::ImageMenuItem *menuEstadisticas;
	
	//Ayuda
	Gtk::ImageMenuItem *menuManualUsuario;



	// Atributos del modelo

	Monitor *monitor;
	Configuracion *serverConfig;
	
public:

	// Constructor
	MenuPrincipal(Monitor *monitor, Configuracion *config);

	// Destructor
	virtual ~MenuPrincipal();

	// Inicia la ejecución de la ventana
	void correr();

	
	// Define tareas a ejecutar en el hilo.
	virtual void run();

protected:
	// Acciones de botones
	
	void on_buttonConfiguracion_clicked();
	void on_buttonSalir_clicked();

	// Acciones del menu
	void on_menuConfiguracion_activate();
	void on_menuSalir_activate();
	void on_menuAdminUsers_activate();
	void on_menuEstadisticas_activate();

	void on_menuManualUsuario_activate();

};

#endif /* MENUPRINCIPAL_H_ */
