//
//  client_manejador_archivos.h
//  CLASE MANEJADORARCHIVOS
//  


#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <string>
#include "common_thread.h"
#include "common_cola.h"



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Inspector : public Thread {
private:

	int intervalo;				// Intervalo de inspección en segundos
	Cola< int > *eventos;		// Cola de eventos


	// Bloquea actividades hasta que haya transcurrido el intervalo de polling
	void alarmaDeInspeccion();

public:

	// Constructor
	Inspector();

	// Destructor
	~Inspector();

	//
	void iniciar();

	// 
	void establecerIntervaloDeInspeccion(int segundos);

	//
	void forzarInspeccion();

	// Define tareas a ejecutar en el hilo.
	// Realiza una inspección cada un intervalo predeterminado.
	virtual void run();
};

#endif
