//
//  client_receptor_de_archivos.h
//  CLASE RECEPTORDEARCHIVOS
//  


#ifndef RECEPTOR_DE_ARCHIVOS_H
#define RECEPTOR_DE_ARCHIVOS_H


#include <string>
class Mutex;
class Lock;



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ReceptorDeArchivos {
private:

	Mutex m;							// Mutex

public:

	// Constructor
	ReceptorDeArchivos();

	// Destructor
	~ReceptorDeArchivos();
};

#endif
