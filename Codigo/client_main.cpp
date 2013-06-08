#include "client_conexion.h"
#include <iostream>

using namespace std;


int main (int argc, char** argv) {
	cout <<"compilado"<<endl;
	Gtk::Main kit(argc, argv);
	conexion a;
	a.correr();
	return 0;

}
