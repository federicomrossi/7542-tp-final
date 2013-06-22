//
//  monitor_interfaz_menuEstadisticas.h
//  CLASE MENUESTADISTICAS
//  


#ifndef MENUESTADISTICAS_H_
#define MENUESTADISTICAS_H_


#include "gtkmm.h"
#include "monitor_canvas.h"
#include "monitor_monitor.h"
#include "monitor_configuracion.h"
// aca hay q agregar al canvas? y cual es la clase canvas? eso tmb hay q agregarlo agarrala de mi tp 5, carpetaa vistas dale 
//ehh ni idea, como gustes xD  si calculo q es common, mmmm noo mejor ponele monitor solo el monitor va a graficar no? si 
// tenes razon al pedo


class MenuEstadisticas : public Gtk::Window {
private:

	// Atributos de la interfaz
	Gtk::Window *main;				 // Ventana Conexion
	

	// Atributos del modelo
	Canvas c;
	Monitor *monitor;
	
public:

	// Constructor
	MenuEstadisticas(Monitor *monitor);

	// Destructor
	virtual ~MenuEstadisticas();

	// Inicia la ejecución de la ventana
	void correr();

};

#endif /* MENUESTADISTICAS_H_ */
