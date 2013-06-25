#include "monitor_graficador.h"
#include <iostream>
#include "monitor_vista.h"
#include "monitor_vista_linea.h"
#include "monitor_vistaFondo.h"

#define B 1
#define KB 1024
#define MB 1048576

Graficador::Graficador() {
	fondo = new VistaFondo();
}

Graficador::~Graficador() {
	vistasLinea.clear();
	delete(fondo);
	delete(indicador);
}

void Graficador::agregarVista(Vista* v) {
	vistasLinea.push_back(v);
	v->setGraficador(this);
	add_events(Gdk::BUTTON_PRESS_MASK);
}

void Graficador::run() {

	// Se selecciona escala de acuerdo a los bytes que ocupados en el servidor
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
			//actualizo la escala actual segun la actual medicion de bytes (B,Kb,MB)
			escalaActual = escalaRequerida(monitor->getBytesOcupados());

			//corro toda lo dibujado a la izquierda
			//refresco las lineas actuales a la escala actual
			list<Vista*>::iterator it;
			for ( it=vistasLinea.begin() ; it != vistasLinea.end(); it++ ){
					((VistaLinea*)(*it))->correrIzquierda(10);
					((VistaLinea*)(*it))->escala = escalaActual; // actualizo la escala de 
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

		// En este ciclo cada 1 segundo se crea una nueva linea, se corre y se actualiza la escala de las lineas 
		// anteriores y se vuelve a ejectuar el evento ondraw-> se dibuja todo de nuevo
		
	}
	

	
}

int Graficador::escalaRequerida(int bytes) {

	if (bytes < KB) return B;
	if (bytes < MB) return KB;
	return MB;
}


void Graficador::correr(){
	this->estado = true;
	this->start();

}
void Graficador::detener(){
	
	this->estado = false;
	this->join();	
}
//aca este es el evento ondraw q se ejecuta y este recorre todos los objetos vista diciendoles "draw"
bool Graficador::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	//el cr (cairorefprt) es donde se puede dibujar
	//le decimos al fondo y al incador q se dibujen
	fondo->draw(cr);
	indicador->draw(cr);
	//le decimos a todas las lineas q se dibujen
	list<Vista*>::iterator it;
	for ( it=vistasLinea.begin() ; it != vistasLinea.end(); it++ ) ((Vista*)(*it))->draw(cr);
	
	return true;
}

void Graficador::quitarVistas() { 
	vistasLinea.clear();
}
