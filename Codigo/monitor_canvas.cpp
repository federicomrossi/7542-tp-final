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
	//esto se ejecuta cuando se le da correr al canvas, ya en otro hilo, entonces aca, podemos ponerfile:///home/fiona/Escritorio/RepoTp/Codigo/cliente/MiArchivoPrueba!!! dentro del while
	//la carga de todas las vistas a partir de lo que dice el monitor, despues ejecutamos lo que rompe la pantalla
	// se ejecuta el draw y se dibujan :D cada 30ms
	// fijatre si no se compi puta madre quedate quieto...fijate sino se compilo ya el anterior ... y te sigue devolviendo string
	
	while (estado){
		//sin querer borramos la logica xDme acgue todo bueno sjajaij es xarpadamente rapido
			if (vistas.size() > 50) {
				//vistas.pop_front();
				vistas.pop_front();
			}
			list<Vista*>::iterator it;
			for ( it=vistas.begin() ; it != vistas.end(); it++ ) ((VistaLinea*)(*it))->correrIzquierda(10);
			//VistaLinea* horizontal = new VistaLinea( 500, 400 - monitor->getBytesOcupados()/1024 , 500 - 10, 400 - monitor->getBytesOcupados()/1024); 
			VistaLinea* vertical = new VistaLinea( 500 - 1, 400 - monitor->getBytesOcupados()/1024 , 500 - 1, 400);

			//this->agregarVista(horizontal);
			this->agregarVista(vertical); // deberia tener los valores invertidos no? dejame pensar xD
			Glib::RefPtr<Gdk::Window> win = get_window();
			if (win) {
				Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
				win->invalidate_rect(r, false);
			}
		sleep(1);
	}
}
//yo no quiero ser mmm ???? pe quuisiera contarte q e tipo me habla toooooooooooodos los dias
//  jes es mas... el martes me vino a buscar a un aula donde estaba boluda casi q te esta acosadno se yo puedo tambienn? :$ ale yo te acoso a vos
// estas de vacaciones y mira como te rompo los huevos mm no lo habia pensado oh dios mio D: JAJAJAorJAJ basta programemos
// como lo de big bang t cuando pensaban no se que en el pizzarron jajajaajajajajajaj es genial..tenemos (ahora q tenes el coso) q grabar
// un video asi mmm despues lo negociamos, con ropa o sin? jajaj jajaja dale ale okok con bueno

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
