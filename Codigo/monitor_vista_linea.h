#ifndef VISTALINEA_H_
#define VISTALINEA_H_

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>

#include <iostream>
#include <cairomm/surface.h>

#include <cairomm/context.h>
#include <cairomm/refptr.h>

#include "monitor_vista.h"

class VistaLinea : public Vista {
public:
	int xini;
	int yini;
	int xfin;
	int yfin;
	int escala;
	VistaLinea(int xini, int yini, int xfin, int yfin);
	virtual ~VistaLinea();
	void correrIzquierda(int x);
	void draw(const Cairo::RefPtr<Cairo::Context>& cr, Gtk::Allocation& allocation);
};

#endif
