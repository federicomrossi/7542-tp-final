//
//  client_receptor_de_archivos.h
//  CLASE RECEPTORDEARCHIVOS
//  


#ifndef RECEPTOR_DE_ARCHIVOS_H
#define RECEPTOR_DE_ARCHIVOS_H


#include <string>
#include "client_manejador_de_archivos.h"
class Mutex;
class Lock;



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ReceptorDeArchivos {
private:

	Mutex m;							// Mutex
	ManejadorDeArchivos *manejadorDeArchivos;		// Manejador de archivos

public:

	// Constructor
	ReceptorDeArchivos(ManejadorDeArchivos *unManejador);

	// Destructor
	~ReceptorDeArchivos();
};

#endif
