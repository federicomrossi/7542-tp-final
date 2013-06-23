#include "monitor_vista_linea.h"

VistaLinea::VistaLinea(int xini, int yini, int xfin, int yfin) {
	this->xini = xini;
	this->yini = yini;
	this->xfin = xfin;
	this->yfin = yfin;
	this->escala = 1;
}

VistaLinea::~VistaLinea() {}
void VistaLinea::correrIzquierda(int x){
	xini = xini - x;
	xfin = xfin - x;
}

void VistaLinea::draw(const Cairo::RefPtr<Cairo::Context>& cr, Gtk::Allocation& allocation) {
	
	cr->save();
	
	if (escala == 1048576 && yini < (escala*100) && yfin < (escala*100)) {

	cr->move_to (xini, 512 - ((yini/escala)*10)/2);
    cr->line_to (xfin, 512 - ((yfin/escala)*10)/2);
	
	}else {

	cr->move_to (xini, 512 - (yini/escala)/2);
    cr->line_to (xfin, 512 - (yfin/escala)/2);
	}
    
    cr->set_line_width (10.0); 
	cr->restore();  // back to opaque black
	cr->stroke();
}
