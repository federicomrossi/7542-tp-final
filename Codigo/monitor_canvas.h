#ifndef CANVAS_H_
#define CANVAS_H_

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
using namespace std;
class Canvas : public Gtk::DrawingArea , Thread {
private:
	list<Vista*> vistas; 
protected:
	bool estado;
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

public:
	Canvas();
	virtual ~Canvas();
	void agregarVista(Vista* v);
	void quitarVistas();
	virtual void run();
	void correr();
	void detener();
	Monitor *monitor;
};

#endif /*CANVAS_H_*/
