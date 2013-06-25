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
	v->setCanvas(this);
	add_events(Gdk::BUTTON_PRESS_MASK);
}

void Graficador::run() {

	// Se selecciona escala de acuerdo a los bytes que ocupados en el servidor
	int escalaActual = escalaRequerida(monitor->getBytesOcupados());
	//Aca definimos una linea inicial las coord xy de cada extremo, el objeto vistaLinea tb guarda la escala
	//entonces cada vez q cada linea se va a dibujar, usa la escala para dividir los bytes ocupados
	//entonces nosotros a las lineas ya existentes en cualquiermomento le cambiamos la escla
	//y la proxima vez q se dibuje se va a dividir por la nueva escala entonces se reposiciona todo sin problema :D
	//ahora lo vemos bien
	VistaLinea* primera = new VistaLinea( 500-10, monitor->getBytesOcupados() , 500, monitor->getBytesOcupados()); 
	primera->escala = escalaActual;
	this->agregarVista(primera);
 	// bueno el tema es asi, esta clase es una drawingarea q permite dibujarle yerbas<-con esta clase me refiero a esta, monitor_canvas si bienbien
 	// paraaa poder organizar un poco esto, las yerbas q se le dibujan las modularizamos en objetos vista
 	//tipo vos aca en el evento ondraw de esta clase podrias dibujar todo directamente y ya
 	//pero seria un lio, ademas ibamos a tener q dibujar un mnonton de lineas asiq directamente
 	//el codigo para dibujar una linea lo separamos en una "vistalinea"ta :)? bien  nada segui dalep
 	//bueno vistaIndicador essssssss el label q te muestra el numerito, basicamente adentro tiene las 10 lineas de codigo
 	//que requiere una drawing area para escribir en ella, pero no da ponerlo aca mismo
 	 //lo mismo vista fondo, ayer el fondo no se msotraba daba error xq lo estaba metiendo en la lista de vistasLinea
	//entonces intentaba correrlo a la izquierda y cuando llegaba a la punta de la lista, lo queria borrar xD y fallaba todonaaaa
	// eso era? q boludossss claro, por eso lo puse en un atributo aparte y funco xD q bien
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
		sleep(1);//esto basicamente lo q hace es cada 1 segundo crear una linea nueva, correr y actualizarle la escala de las viejas y romper la drawing para que se redibuje
	//bien..ahora hay q ver la linea o el fondo, //no primero esas 3 lineas rompen la drawing, cuando pasa eso
		//se vuelve a ejecutar el evento ondraw

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
//evento ondraw que hace, agarra y le dice
bool Graficador::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	//el cr (cairorefprt) es donde se puede dibujar
	//le decimos al fondo y al incador q se dibujen
	fondo->draw(cr,allocation);
	indicador->draw(cr,allocation);
	//le decimos a todas las lineas q se dibujen
	list<Vista*>::iterator it;
	for ( it=vistasLinea.begin() ; it != vistasLinea.end(); it++ ) ((Vista*)(*it))->draw(cr);
	
	return true;
}

void Graficador::quitarVistas() { 
	vistasLinea.clear();
}
