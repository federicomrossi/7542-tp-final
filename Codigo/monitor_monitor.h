//
//  monitor_monitor.h
//  CLASE MONITOR
//  


#ifndef MONITOR_H
#define MONITOR_H

#include "common_archivoTexto.h"
#include "monitor_receptorDatos.h"
#include "common_convertir.h"


/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Monitor {
private:

	// Atributos generales
	ArchivoTexto* archivoLog;
	int carpetasActivas;
	int clientesConectados;


	Receptor* receptor;

protected: 
	int posLecturaLog;

public:

	// Constructor
	Monitor(Receptor* receptor);

	// Destructor
	~Monitor();
	
	// Actualizar valores

	void actualizarValores();
	//retorna un string con la ultima linea del buffer leida
	string* getBufferLog();

	




	
	void cambiarIntervaloPolling(unsigned int intervalo);

};

#endif
