//
//  monitor_monitor.h
//  CLASE MONITOR
//  


#ifndef MONITOR_H
#define MONITOR_H

#include "common_archivoTexto.h"


/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Monitor {
private:

	// Atributos generales
	ArchivoTexto* archivoLog;
	int carpetasActivas;
	int clientesConectados;

protected: 
	int posLecturaLog;

public:

	// Constructor
	Monitor();

	// Destructor
	~Monitor();
	//retorna un string con la ultima linea del buffer leida
	string* getBufferLog();

	
	void cambiarIntervaloPolling(unsigned int intervalo);

};

#endif
