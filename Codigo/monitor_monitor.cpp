
#include <iostream>
#include <sstream>
#include "monitor_monitor.h"


/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Monitor::Monitor() {
	this->posLecturaLog = 0;
	this->carpetasActivas = 0;
	this->clientesConectados = 0;
}

string* Monitor::getBufferLog() {
	string* cadena = new string();
	this->archivoLog = new ArchivoTexto("log",0);
	this->archivoLog->leerLinea(*cadena, this->posLecturaLog);
	cout<< "cadena leida  "<< *cadena <<endl;
	delete(this->archivoLog);
	int aux = 0;
	aux = (int) cadena->size() + 1 + this->posLecturaLog;
	//analizo si estoy en fin de archivo, sino avanzo en la lectura
	if (this->posLecturaLog + 2 != aux) { 
		this->posLecturaLog = (int) cadena->size() + 1 + this->posLecturaLog;
	}
	return cadena;

}

// Destructor
Monitor::~Monitor() {
}



void Monitor::cambiarIntervaloPolling(unsigned int intervalo) {

}
