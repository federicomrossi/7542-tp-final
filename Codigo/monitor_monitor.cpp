
#include <iostream>
#include <sstream>
#include "common_parser.h"
#include "monitor_monitor.h"


/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Monitor::Monitor(Receptor* receptor) {
	this->posLecturaLog = 0;
	this->carpetasActivas = 0;
	this->clientesConectados = 0;
	this->receptor = receptor;
	this->estado = 0;
}

Receptor* Monitor::getReceptor(){
	return receptor;
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

void Monitor::actualizarValores() {
	Lista<std::string> nuevos = this->receptor->getValores();
	if ((this->receptor->getEstadoConexion()) == 1) {
		this->estado = true;
	} else {
	 this->estado = false;
	}
	this->clientesConectados = Convertir::stoi(nuevos[0]); 
	this->carpetasActivas = Convertir::stoi(nuevos[1]);
	this->bytesOcupados = Convertir::stoi(nuevos[2]);
}

bool Monitor::getEstadoConexion(){
	return this->estado;
}

string Monitor::getClientesConectados() {
	return Convertir::itos(this->clientesConectados);
}
string Monitor::getCarpetasActivas() {
	return Convertir::itos(this->carpetasActivas);
}

int Monitor::getBytesOcupados() {
	return this->bytesOcupados;
}

void Monitor::getUsuarios(){
	string usuarios;
	string query = "GET-USER-LIST"; // hay q meter la constante que corresponda dsd protocolo.h
	receptor->enviarMensaje(query);
	receptor->recibirMensaje(usuarios); // tenemos el mensaje
	Lista <std::string> aux;
 	string instruccion; 
 	string args;
 	Parser::parserInstruccion(usuarios, instruccion, args);
	Parser::dividirCadena(args, &aux, COMMON_DELIMITER[0]);
	this->usuarios = aux;
}


// Destructor
Monitor::~Monitor() {
}



void Monitor::cambiarIntervaloPolling(unsigned int intervalo) {

}
