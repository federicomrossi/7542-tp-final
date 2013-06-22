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
	int bytesOcupados;
	int estado;
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
	// Retorna un string con la ultima linea del buffer leida
	string* getBufferLog();

	// Retorna un string con la cantidad de clientes conectados
	string getClientesConectados();
	// Retorna un string con la cantidad de carpetas activas
	string getCarpetasActivas();

	int getBytesOcupados();

	// Retorna si el servidor sigue levantado o no
	bool getEstadoConexion();

	
	
	void cambiarIntervaloPolling(unsigned int intervalo);

};

#endif
