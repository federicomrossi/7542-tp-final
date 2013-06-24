#ifndef VISTAFONDO_H_
#define VISTAFONDO_H_

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>

#include <iostream>
#include <cairomm/surface.h>

#include <cairomm/context.h>
#include <cairomm/refptr.h>

#include "monitor_vista.h"

class VistaFondo : public Vista {
private:
	Glib::RefPtr<Gdk::Pixbuf> imagen;
    
public:
	VistaFondo();
	virtual ~VistaFondo();
	void draw(const Cairo::RefPtr<Cairo::Context>& cr, Gtk::Allocation& allocation);
	
};

#endif /*VISTAFONDO_H_*/
