#include "monitor_canvas.h"
#include <iostream>
#include "monitor_vista.h"
#include "monitor_vista_linea.h"
#include "monitor_vistaFondo.h"

#define B 1
#define KB 1024
#define MB 1048576

Canvas::Canvas() {
	fondo = new VistaFondo();
}

Canvas::~Canvas() {
	vistasLinea.clear();
	delete(fondo);
	delete(indicador);
}

void Canvas::agregarVista(Vista* v) {
	vistasLinea.push_back(v);
	v->setCanvas(this);
	add_events(Gdk::BUTTON_PRESS_MASK);
}

void Canvas::run() {

	int escalaActual = escalaRequerida(monitor->getBytesOcupados());

	VistaLinea* primera = new VistaLinea( 500-10, monitor->getBytesOcupados() , 500, monitor->getBytesOcupados()); 
	primera->escala = escalaActual;
	this->agregarVista(primera);

	indicador = new VistaIndicador(monitor);

	while (estado){
			
			//si se llena el grafico, borro el primero
			if (vistasLinea.size() > 50) {
				vistasLinea.pop_front();
			}
			//actualizo la escala actual segun la actual medicion de bytes
			escalaActual = escalaRequerida(monitor->getBytesOcupados());

			//corro toda lo dibujado a la izquierda
			//refresco las lineas actuales a la escala actual
			list<Vista*>::iterator it;
			for ( it=vistasLinea.begin() ; it != vistasLinea.end(); it++ ){
					((VistaLinea*)(*it))->correrIzquierda(10);
					((VistaLinea*)(*it))->escala = escalaActual;
			}
			//creo la linea para la actual medicion
			VistaLinea* horizontal = new VistaLinea( 500 - 10, ((VistaLinea*)vistasLinea.back())->yfin , 500, monitor->getBytesOcupados()); 
			//cargo la linea en el lienzo 
			this->agregarVista(horizontal);
			//rompo la pantalla para que se ejecute el evento "on_draw" de nuevo
			Glib::RefPtr<Gdk::Window> win = get_window();
			if (win) {
				Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
				win->invalidate_rect(r, false);
			}
		sleep(1);
	}
	

	
}

int Canvas::escalaRequerida(int bytes) {

	if (bytes < KB) return B;
	if (bytes < MB) return KB;
	return MB;
}


void Canvas::correr(){
	this->estado = true;
	this->start();

}
void Canvas::detener(){
	
	this->estado = false;
	this->join();	
}
bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	Gtk::Allocation allocation = get_allocation();
	fondo->draw(cr,allocation);
	indicador->draw(cr,allocation);
	list<Vista*>::iterator it;
	for ( it=vistasLinea.begin() ; it != vistasLinea.end(); it++ ) ((Vista*)(*it))->draw(cr,allocation);
	
	return true;
}

void Canvas::quitarVistas() { 
	vistasLinea.clear();
}
