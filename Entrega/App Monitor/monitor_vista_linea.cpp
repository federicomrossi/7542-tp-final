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

	// dividimos por cuatro -> 1024 / 4 = 256 que es la altura del form (logramos q los resultados sean representables)
	
	// ahora haces si la escala q viene *100  es menor a yini y la escala q viene *100 es menor yfin entonces
	
	//GRAFICAR A ESCALA
	//CASO A) cualquier representacion menos ( 0 a 100mb) 
	// se toma el valor se lo divide por la escala y por 4 para que entre en el grafico
	// y luego con el indicador, se muestra en que escala  se trabaja

	//CASO B) Representar 1 a 100mb
	//entro en el if	

	if (escala == 1048576 && yini < (escala*100) && yfin < (escala*100)) {
        // para que las lineas se desparramen mejor en el grafico de 256 bytes,
		//ya que no vamos a distribuir de 1 a 1024 valores sino que de 1 a 100, multiplico x 10
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
