//
//  monitor_configuracion.cpp
//  CLASE CONFIGURACION
//  


#include <iostream>
#include <string>
#include <sstream>
#include "common_convertir.h"
#include "monitor_configuracion.h"

using namespace std;




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Configuracion::Configuracion() {

}

// Destructor
Configuracion::~Configuracion() { }


// Devuelve el valor especifico que se necesita
std::string Configuracion::getInfo(std :: string &cadena) {
	string val;
	unsigned pos = cadena.find(CONFIG_SEPARATOR);         
	val = cadena.substr (pos+1);
	return val;
}

// Devuelve el puerto del servidor.
int Configuracion::obtenerPuerto() {
	string* cadena = new string();
	this->Archivo = new ArchivoTexto (CONFIG_DIR + CONFIG_FILENAME + CONFIG_FILE_EXT,0);
	bool estado = false;
	while(estado == (this->Archivo->leerLinea(*cadena, '\n', CONFIG_P_PORT)));
	string result = getInfo(*cadena);
	delete(this->Archivo);
	delete(cadena);
	return Convertir:: stoi(result);
}

//Devuelve el intervalo de actualizacion de informacion del monitor
int Configuracion::obtenerTiempo() {
	string* cadena = new string();
	this->Archivo = new ArchivoTexto (CONFIG_DIR + CONFIG_FILENAME + CONFIG_FILE_EXT,0);
	bool estado = false;
	while(estado == (this->Archivo->leerLinea(*cadena, '\n', CONFIG_P_TIME)));
	string result = getInfo(*cadena);
	delete(this->Archivo);
	delete(cadena);
	return Convertir:: stoi(result);
}


std::string Configuracion::obtenerHost() {
	string* cadena = new string();
	this->Archivo = new ArchivoTexto (CONFIG_DIR + CONFIG_FILENAME + 
		CONFIG_FILE_EXT,0);
	bool estado = false;
	while(estado == (this->Archivo->leerLinea(*cadena, '\n', CONFIG_P_HOST)));
	string result = getInfo(*cadena);
	delete(this->Archivo);
	delete(cadena);
	return result;
}

string Configuracion::obtenerLog() {

	string* cadena = new string();
	this->Archivo = new ArchivoTexto (CONFIG_DIR + CONFIG_FILENAME + CONFIG_FILE_EXT,0);
	bool estado = false;
	while(estado == (this->Archivo->leerLinea(*cadena, '\n', CONFIG_LOG)));
	string result = getInfo(*cadena);
	delete(this->Archivo);
	delete (cadena);

	return result; 
}

void Configuracion::guardarCambios(string puerto, string host, string time) {


this->Archivo = new ArchivoTexto(CONFIG_DIR + CONFIG_FILENAME + 
		CONFIG_FILE_EXT,1);
	
	string* aux = new string();
	*aux += "#SETTINGS MONITOR";
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
	*aux += CONFIG_P_TIME;
	*aux += CONFIG_SEPARATOR;
	*aux += time;
	*aux += '\n';
	this->Archivo->escribir(*aux);

	delete(this->Archivo);
	delete(aux);

}

