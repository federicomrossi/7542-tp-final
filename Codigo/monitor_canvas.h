#ifndef CANVAS_H_
#define CANVAS_H_

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <list>
#include <iostream>
#include <map>
#include <sigc++/signal.h>


#include "monitor_vista.h"
#include "common_thread.h"
#include <cairomm/surface.h>
#include <cairomm/context.h>
#include <cairomm/refptr.h>
#include "monitor_monitor.h"
using namespace std;
class Canvas : public Gtk::DrawingArea , Thread {
private:
	list<Vista*> vistas; 
protected:
	bool estado;
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
//funca? q cosa=?? ahora compila si... y grafica como venia haciendo ..y salta de escala lo vi :D loo quee
	//quedaria retocar ess q la escala de MB no sea hasta 1gb sino q sea mas chica asi se ve mejor? o ta bien asi
	// mm para modificarlo tendriamos q agregar una constante mas... 
	// lo que no se es q carajo inventar para q el grafico...sea legible xq nosotros lo etnendemos pero....
	//jajaja ahora vemos eso lo de la escala, no no es solo agregar una constante, hay q modificar el metodo q selecciona escalas
	//y no se q va  apasar porq el grafico esta pensado para distribuir de 0 a 1024
public:
	Canvas();
	virtual ~Canvas();
	void agregarVista(Vista* v);
	void quitarVistas();
	int escalaRequerida(int bytes);
	virtual void run();
	void correr();
	void detener();
	Monitor *monitor;
};

#endif /*CANVAS_H_*/
