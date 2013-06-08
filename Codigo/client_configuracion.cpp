//
//  client_configuracion.h
//  CLASE CONFIGURACION
//  


#include <iostream>
#include <string>
#include <sstream>
#include "common_convertir.h"
#include "client_configuracion.h"









/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Configuracion::Configuracion() { }


// Destructor
Configuracion::~Configuracion() { }


// Devuelve el directorio en el que se desea sincronizar.
std::string Configuracion::obtenerDirectorio() {
	return "cliente";
}


// Devuelve el host del servidor.
std::string Configuracion::obtenerHost() {
	return "127.0.0.1";
}


// Devuelve el puerto del servidor.
int Configuracion::obtenerPuerto() {
	std::string sPuerto = "8000";
	return Convertir::stoi(sPuerto);
}


// Devuelve el intervalo de polling en segundos.
int Configuracion::obtenerIntervaloDePolling() {
	return 5;
}


// Actualiza el directorio en el que se desea sincronizar.
void Configuracion::asignarDirectorio(std::string directorio) {

}


// Actualiza el host del servidor.
void Configuracion::asignarHost(std::string host) {

}


// Actualiza el puerto del servidor.
void Configuracion::asignarPuerto(int puerto) {

}


// Actualiza el intervalo de polling, el cual debe estar en segundos.
void Configuracion::asignarIntervaloDePolling(int intervalo) {

}
