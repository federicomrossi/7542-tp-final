#ifndef GRAFICADOR_H_
#define GRAFICADOR_H_

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <list>
#include <iostream>
#include <map>
#include <sigc++/signal.h>

#include "monitor_vista.h"
#include "common_thread.h"
#include <cairomm/surface.h>
#include <cairomm/context.h>
#include <cairomm/refptr.h>
#include "monitor_monitor.h"
#include "monitor_vistaIndicador.h"
using namespace std;

class Graficador : public Gtk::DrawingArea , Thread {
private:
	list<Vista*> vistasLinea;
	Vista* fondo;
	VistaIndicador* indicador;
protected:
	bool estado;
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

public:
	Graficador();
	virtual ~Graficador();
	void agregarVista(Vista* v);
	void quitarVistas();
	int escalaRequerida(int bytes);
	virtual void run();
	void correr();
	void detener();
	Monitor *monitor;
};

#endif /*GRAFICADOR_H_*/
