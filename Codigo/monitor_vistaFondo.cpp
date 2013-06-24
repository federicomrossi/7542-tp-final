#include "monitor_vistaFondo.h"
#include <iostream>

VistaFondo::VistaFondo(){
	
	std::string imgPath = "interfaz/imagenes/fondo.png";
	this->imagen = Gdk::Pixbuf::create_from_file(imgPath,500,256,true);	

}

VistaFondo::~VistaFondo() {
}

void VistaFondo::draw(const Cairo::RefPtr<Cairo::Context>& cr, Gtk::Allocation& allocation) {
	
	cr->save(); 

	Gdk::Cairo::set_source_pixbuf(cr, this->imagen, 0, 0);

	cr->fill();	
	cr->paint();
	cr->restore();
	cr->stroke();
}