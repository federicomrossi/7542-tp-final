//
//  server_interfaz_principal.h
//  CLASE MENUPRINCIPAL
//  


#ifndef MENUPRINCIPAL_H_
#define MENUPRINCIPAL_H_


#include "gtkmm.h"
#include "server_servidor.h"
#include "server_configuracion.h"



class MenuPrincipal : public Gtk::Window {
private:

	// Atributos de la interfaz
	Gtk::Window *main;				 // Ventana Conexion
	
	Gtk::Label  *estado;			 // Estado del servidor
	Gtk::Label  *clientesConectados;
	Gtk::Label	*carpetasActivas;
	
	Gtk::Button *botonIniciar;		 // Botón que levanta el servidor
	Gtk::Button *botonConfiguracion; // Botón Salir
	Gtk::Button *botonSalir;
	
	//Atributos del menu
	//Monitor
	Gtk::ImageMenuItem *menuConfiguracion;
	Gtk::ImageMenuItem *menuSalir;
	//Opciones
	Gtk::ImageMenuItem *menuAdminUsers;
	Gtk::ImageMenuItem *menuEstadisticas;
	Gtk::ImageMenuItem *menuLog;
	//Ayuda
	Gtk::ImageMenuItem *menuManualUsuario;
	Gtk::ImageMenuItem *menuAyuda;


	// Atributos del modelo

	Servidor *servidor;
	Configuracion *serverConfig;


	
public:

	// Constructor
	MenuPrincipal(Servidor *servidor, Configuracion *config);

	// Destructor
	virtual ~MenuPrincipal();

	// Inicia la ejecución de la ventana
	void correr();

protected:
	// Acciones de botones
	void on_buttonIniciar_clicked();
	void on_buttonConfiguracion_clicked();
	void on_buttonSalir_clicked();

	// Acciones del menu
	void on_menuConfiguracion_activate();
	void on_menuSalir_activate();
	void on_menuAdminUsers_activate();
	void on_menuEstadisticas_activate();
	void on_menuLog_activate();
	void on_menuManualUsuario_activate();
	void on_menuAyuda_activate();
};

#endif /* MENUPRINCIPAL_H_ */
