#include "monitor_vistaIndicador.h"
#include "pangomm.h"
#include <iostream>
#define B 1
#define KB 1024
#define MB 1048576

VistaIndicador::VistaIndicador(Monitor* monitor) {
	this->monitor = monitor;

}

VistaIndicador::~VistaIndicador() {
}

void VistaIndicador::draw(const Cairo::RefPtr<Cairo::Context>& cr, Gtk::Allocation& allocation) {
	
	cr->save();
	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_BOLD);
	font.set_absolute_size(20*Pango::SCALE);



	Glib::ustring asdf = Convertir::itos(monitor->getBytesOcupados()/escala(monitor->getBytesOcupados())) + escalaStr(monitor->getBytesOcupados()); // bien bien 
	Gtk::DrawingArea win;
	Glib::RefPtr<Pango::Layout> layout = win.create_pango_layout(asdf);
	layout->set_font_description(font);
	
	cr->move_to(5,5);
	cr->set_source_rgb(255.0, 204.0, 53.0);
	layout->show_in_cairo_context(cr);
	
}
int VistaIndicador::escala(int bytes) {
	// en q escala esta imprimiendo ...?¿?¿?¿?¿?¿ nunca guardamos antes de que compiles xD
	//hasta 100 mb, sale en mb, mas de 100, sale en mb100 mmmmmmmmmmmmmm q esta mal aca a ver
	if (bytes < KB) return B;
	if (bytes < MB) return KB;
	return MB;
}

std::string VistaIndicador::escalaStr(int bytes) {
	
	if (bytes < KB) return "B";
	if (bytes < MB) return "KB";
	return "MB";
}
