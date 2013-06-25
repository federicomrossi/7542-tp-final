#ifndef VISTA_H_
#define VISTA_H_

#include <iostream>


#include <gtkmm.h>
#include <gtkmm/drawingarea.h>

#include <cairomm/surface.h>
#include <cairomm/context.h>
#include <cairomm/refptr.h>


class Graficador;

class Vista {	
protected:
	Graficador* graficador;

public:
	Vista();
	virtual ~Vista();
	virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr)=0;
	void setGraficador(Graficador* graficador);
};

#endif /*VISTA_H_*/



