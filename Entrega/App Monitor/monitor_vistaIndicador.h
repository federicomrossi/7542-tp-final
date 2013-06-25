#ifndef VISTAINDICADOR_H_
#define VISTAINDICADOR_H_

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>

#include <iostream>
#include <cairomm/surface.h>

#include <cairomm/context.h>
#include <cairomm/refptr.h>

#include "monitor_vista.h"
#include "monitor_monitor.h"

class VistaIndicador : public Vista {  
private:
	Monitor* monitor;
	int escala(int bytes);
	std::string escalaStr(int bytes);
public:
	VistaIndicador(Monitor* monitor);
	virtual ~VistaIndicador();
	void draw(const Cairo::RefPtr<Cairo::Context>& cr);
};

#endif /*VISTAINDICADOR_H_*/
