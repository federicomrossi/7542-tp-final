//
//  server_configuracion.h
//  CLASE CONFIGURACION
//  


#include <iostream>
#include <string>
#include <sstream>
#include "common_convertir.h"
#include "server_configuracion.h"

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
	unsigned pos = cadena.find(CONFIG_SEPARATOR);         // position of "=" in cadena
	val = cadena.substr (pos+1);
	return val;
}

// Devuelve el puerto del servidor.
int Configuracion::obtenerPuerto() {
	string* cadena = new string();
	this->Archivo = new ArchivoTexto (CONFIG_DIR + CONFIG_FILENAME + 
		CONFIG_FILE_EXT,0);
	bool estado = false;
	while(estado == (this->Archivo->leerLinea(*cadena, '\n', CONFIG_P_PORT)));
	string result = getInfo(*cadena);
	delete(this->Archivo);
	delete(cadena);
	return Convertir:: stoi(result);
}

// Devuelve el path de los clientes en el servidor
string Configuracion::obtenerHost() {

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


string Configuracion::obtenerPath() {

	string* cadena = new string();
	this->Archivo = new ArchivoTexto (CONFIG_DIR + CONFIG_FILENAME + 
		CONFIG_FILE_EXT,0);
	bool estado = false;
	while(estado == (this->Archivo->leerLinea(*cadena, '\n', CONFIG_P_PATH)));
	string result = getInfo(*cadena);
	delete(this->Archivo);
	delete(cadena);
	return result;

}

void Configuracion::guardarCambios(string puerto, string host, string path) {

	this->Archivo = new ArchivoTexto(CONFIG_DIR + CONFIG_FILENAME + 
		CONFIG_FILE_EXT,1);
	string* aux = new string();
	*aux += "#SETTINGS SERVER";
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
	*aux += CONFIG_P_PATH;
	*aux += CONFIG_SEPARATOR;
	*aux += path;
	*aux += '\n';
	this->Archivo->escribir(*aux);

	delete(this->Archivo);
	delete(aux);

}

