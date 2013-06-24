
#include <iostream>
#include <sstream>
#include "common_parser.h"
#include "monitor_monitor.h"
#include "common_protocolo.h"


/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Monitor::Monitor(Receptor* receptor) {
	this->posLecturaLog = 0;
	this->carpetasActivas = 0;
	this->clientesConectados = 0;
	this->receptor = receptor;

}

Receptor* Monitor::getReceptor(){
	return receptor;
}


void Monitor::actualizarValores() {
	Lista<std::string> nuevos = this->receptor->getValores();
	if ((this->receptor->getEstadoConexion()) == true) {
		this->estado = true;
		this->clientesConectados = Convertir::stoi(nuevos[0]); 
		this->carpetasActivas = Convertir::stoi(nuevos[1]);
		this->bytesOcupados = Convertir::stoi(nuevos[2]);
	} else {
	 	this->estado = false;
	}
	
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

	string query = M_SERVER_USER_LIST_REQUEST; // hay q meter la constante que corresponda dsd protocolo.h
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

