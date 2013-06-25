#include <iostream>
#include <string>
#include "monitor_interfaz_estadisticas.h"



MenuEstadisticas::MenuEstadisticas(Monitor *monitor) {
	// Cargamos la ventana pr
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

	
	// Cargamos elementos
	refBuilder->add_from_file("./interfaz/monitor_estadisticas.glade");
	refBuilder->get_widget("main", this->main); // linkeo el form en blanco donde se muestra el grafico
//Bueno, esta es la ventana en si, MenuEstadisticas hereda de windows, como las otras ventanas que tenias
	//la diferencia esq en las otras vos definias todo en el .glade, aca en el glade no tenes nada practicamente
	//salvo el main, la ventana en si digamos
	//lo que hacemos es main->add(c); ahi agregarle un objeto "canvas" que es una clase que hiciste vos que encapsula la clase DrawingArea
	//que es una clase que provee cairomm para que dibujes la fruta q quieras, pero tiene un uso medio turbio asiq la encapsulamos
	// ahora la miramos//hasta ahora estamos? vos anda preguntandome todo si, hasta ahora estamos bien,
	// tenemos la libreria cairomm q con la calse drawingarea te permite dibujar..como la clase es compleja la encapsulamos y usamos solo lo 
	// una drawingarea es como un boton que agregas en el glade digamos, nomas q en vez de escucharle un click o tomarle el texto
	// le podes dibujar cosas
	//ahora encapsularlo en un "canvas" te permite tambien hacer q este herede de la clse hilo, entonces haces que este 
	//drawingarea se refresque por si misma sin bloquearte el form bien!..pregunta xq canvas??mmm creo que la libreria 
	//donde esta la drawing se llama canvas a ver ya te digo, a mi me gustaba xq sisi pero ya veo q tengo un corrector q se acuerda de tu 
	// tp y me las corto y bueno cambiale el nombre poneleeeeeeee mmmmmmmmmmmmm no se xD canvas me re copa suenjajaa aaj ajcaballeros del zoodiaco
	// vos no estas bien..yo no dormi pero vos no estas bienjajajaaja le ponemos graficador?dale. bueno
	// sigamos...cuando aparece le ponemos, aca q carajo es C anda al .h de esta clase


	main->show_all_children();
	this->monitor = monitor;
	this->c.monitor = monitor;
}

void MenuEstadisticas::correr() {

	//entonces aca cargas la drawing(canvas) a la ventana(main),corres  el hilo del canvas, le das run para q se muestre la ventana
	//entonces de aca saltamos al "run" del hilo que lanza c.correr()
	c.show_all();
	main->add(c);

	c.correr();
	Gtk::Main::run(*main);
	c.detener();
	main->hide();
} 

MenuEstadisticas::~MenuEstadisticas() { }
