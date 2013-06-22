#include "monitor_vista_linea.h"

VistaLinea::VistaLinea(int xini, int yini, int xfin, int yfin) {
	this->xini = xini;
	this->yini = yini;
	this->xfin = xfin;
	this->yfin = yfin;

}

VistaLinea::~VistaLinea() {}

void VistaLinea::correrIzquierda(int x){
	xini = xini - x;
	xfin = xfin - x;
}

void VistaLinea::draw(const Cairo::RefPtr<Cairo::Context>& cr, Gtk::Allocation& allocation) {
	//aca es donde dibuja la cosa mm q
	//const int height = allocation.get_height();
	//const int lesser = obj->getFigura()->getAncho()*2;
	cr->save();

	cr->move_to (xini, yini);
    cr->line_to (xfin, yfin);
	cr->set_line_width (20.0);
  	//tengo la teoria de que el hilo en el canvas no hace falta, solo faltaba el show_all pruebo sacarlo comentando codigo :D
	cr->restore();  // back to opaque black
	cr->stroke();
}
