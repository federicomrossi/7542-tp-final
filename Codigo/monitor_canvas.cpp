#include "monitor_canvas.h"
#include <iostream>
#include "monitor_vista.h"
#include "monitor_vista_linea.h"
Canvas::Canvas() {
}

Canvas::~Canvas() {
}

void Canvas::agregarVista(Vista* v) {
	vistas.push_back(v);
	v->setCanvas(this);
	add_events(Gdk::BUTTON_PRESS_MASK);
}

void Canvas::run() {
	//esto se ejecuta cuando se le da correr al canvas, ya en otro hilo, entonces aca,
	
	while (estado){
		
			if (vistas.size() > 120) {
				//vistas.pop_front();
				vistas.pop_front();
			}
			list<Vista*>::iterator it;
			for ( it=vistas.begin() ; it != vistas.end(); it++ ) ((VistaLinea*)(*it))->correrIzquierda(5);
			VistaLinea* horizontal = new VistaLinea( 500, 400 - monitor->getBytesOcupados()/1024 , 500 - 5, 400 - monitor->getBytesOcupados()/1024); 
			VistaLinea* vertical = new VistaLinea( 500 - 5, 400 - monitor->getBytesOcupados()/1024 , 500 - 5, 400 );

			this->agregarVista(horizontal);
			this->agregarVista(vertical); 
			Glib::RefPtr<Gdk::Window> win = get_window();
			if (win) {
				Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
				win->invalidate_rect(r, false);
			}
		sleep(1);
	}
}


void Canvas::correr(){
	this->estado = true;
	this->start();
}
void Canvas::detener(){
	this->estado = false;	
}
bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	Gtk::Allocation allocation = get_allocation();
	list<Vista*>::iterator it;
	for ( it=vistas.begin() ; it != vistas.end(); it++ ) ((Vista*)(*it))->draw(cr,allocation);
	
	return true;
}

void Canvas::quitarVistas() { 
	vistas.clear();
}
