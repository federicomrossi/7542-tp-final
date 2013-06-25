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

void VistaIndicador::draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	// se selecciona formato (font, tamaño y color)
	// El objeto Font es la fuente
	cr->save();
	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_BOLD);
	font.set_absolute_size(20*Pango::SCALE);
	// Instanciamos el Layout, dandole un texto y un font
	// calculamos el numero de bytes / escala + rotulo de escala
	Glib::ustring aux = Convertir::itos(monitor->getBytesOcupados()/escala(monitor->getBytesOcupados())) + escalaStr(monitor->getBytesOcupados());
	 
	Gtk::DrawingArea win;
	Glib::RefPtr<Pango::Layout> layout = win.create_pango_layout(aux);
	//ponemos el layout (label) en la drawing.
	layout->set_font_description(font);
	
	cr->move_to(5,5);
	cr->set_source_rgb(255.0, 204.0, 53.0);
	layout->show_in_cairo_context(cr);
	
}
int VistaIndicador::escala(int bytes) {

	if (bytes < KB) return B;
	if (bytes < MB) return KB;
	return MB;
}

std::string VistaIndicador::escalaStr(int bytes) {
	
	if (bytes < KB) return "B";
	if (bytes < MB) return "KB";
	return "MB";
}
