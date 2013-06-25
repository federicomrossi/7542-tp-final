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

void VistaLinea::draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	
	cr->save();

	if (escala == 1048576 && yini < (escala*100) && yfin < (escala*100)) {

	cr->move_to (xini, 256 - ((yini/escala)*10)/4);
    cr->line_to (xfin, 256 - ((yfin/escala)*10)/4);
	
	}else {

	cr->move_to (xini, 256 - (yini/escala)/4);
    cr->line_to (xfin, 256 - (yfin/escala)/4);
	}
	cr->set_source_rgb(255.0, 204.0, 53.0);

    cr->stroke();
	cr->restore();  // back to opaque black


// El evento on_draw de graficador llama a los eventos draw de todas las vistas que se quieren mostrar, 
// pasandole el objeto cr, en el cual se dibuja
	
}
