

#ifndef MAINCLIENTE_H
#define MAINCLIENTE_H

#include <string>

using namespace std;

class MainCliente {

	
public:

	// Constructor
	MainCliente();

	// Destructor
	~MainCliente();

	int conectar(string user, string pass);
	
};

#endif
