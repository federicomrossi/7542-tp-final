//
//  client_configuracion.h
//  CLASE CONFIGURACION
//  


#include <iostream>
#include <string>
#include <sstream>
#include "common_convertir.h"
#include "client_configuracion.h"

using namespace std;







/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Configuracion::Configuracion() {

}


// Destructor
Configuracion::~Configuracion() { }



std::string Configuracion::getInfo(std :: string &cadena) {
	string val;
	unsigned pos = cadena.find(CONFIG_SEPARATOR);         // position of "=" in cadena
	val = cadena.substr (pos+1);
	return val;
}

// Devuelve el directorio en el que se desea sincronizar.
std::string Configuracion::obtenerDirectorio() {
	string* cadena = new string();
	this->Archivo = new ArchivoTexto ("Settings.txt",0);
	bool estado = false;
	while(estado == (this->Archivo->leerLinea(*cadena, '\n', CONFIG_P_DIR)));
	string result = getInfo(*cadena);
	delete(this->Archivo);
	delete(cadena);
	return result;
}

// Devuelve el host del servidor.
std::string Configuracion::obtenerHost() {
	string* cadena = new string();
	this->Archivo = new ArchivoTexto ("Settings.txt",0);
	bool estado = false;
	while(estado == (this->Archivo->leerLinea(*cadena, '\n', CONFIG_P_HOST)));
	string result = getInfo(*cadena);
	delete(this->Archivo);
	delete(cadena);
	return result;
}


// Devuelve el puerto del servidor.
int Configuracion::obtenerPuerto() {
	string* cadena = new string();
	this->Archivo = new ArchivoTexto ("Settings.txt",0);
	bool estado = false;
	while(estado == (this->Archivo->leerLinea(*cadena, '\n', CONFIG_P_PORT)));
	string result = getInfo(*cadena);
	delete(this->Archivo);
	delete(cadena);
	return Convertir:: stoi(result);
}

// Devuelve el intervalo de polling en segundos.
int Configuracion::obtenerIntervaloDePolling() {
	string* cadena = new string();
	this->Archivo = new ArchivoTexto ("Settings.txt",0);
	bool estado = false;
	while(estado == (this->Archivo->leerLinea(*cadena, '\n', CONFIG_P_POLL)));
	string result = getInfo(*cadena);
	delete(this->Archivo);
	delete(cadena);
	return Convertir:: stoi(result);
}

void Configuracion::guardarCambios(string host,string puerto,string dir, string polling) {

	this->Archivo = new ArchivoTexto("Settings.txt",1);
	string* aux = new string();
	*aux += "#SETTINGS USER";
	*aux += '\n';
	this->Archivo->escribir(*aux);
	aux->clear();
	*aux += CONFIG_P_HOST;
	*aux += CONFIG_SEPARATOR;
	*aux += host;
	*aux += '\n';
	this->Archivo->escribir(*aux);

	aux->clear();
	*aux += CONFIG_P_PORT;
	*aux += CONFIG_SEPARATOR;
	*aux += puerto;
	*aux += '\n';
	this->Archivo->escribir(*aux);

	aux->clear();
	*aux += CONFIG_P_DIR;
	*aux += CONFIG_SEPARATOR;
	*aux += dir;
	*aux += '\n';
	this->Archivo->escribir(*aux);

	aux->clear();
	*aux += CONFIG_P_POLL;
	*aux += CONFIG_SEPARATOR;
	*aux += polling;
	*aux += '\n';
	this->Archivo->escribir(*aux);

	delete(this->Archivo);
	delete(aux);

}

