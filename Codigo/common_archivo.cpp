//  
//  common_archivo.cpp
//  CLASE ARCHIVO
//  
//  Clase que representa a un archivo, guardando metadata de este
//  


#include "common_archivo.h"


// Constructor
Archivo::Archivo() { }

// Constructor
Archivo::Archivo(std::string nombreDeArchivo) : nombre(nombreDeArchivo) { }


// Destructor
Archivo::~Archivo() { }


// Asigna un nombre al archivo
void Archivo::asignarNombre(std::string nombre) {
	this->nombre = nombre;
}


// Actualiza el hash del archivo
void Archivo::asignarHash(std::string hash) {
	this->hash = hash;
}


// Actualiza la fecha de la última modificación hecha sobre el archivo.
void Archivo::asignarFechaDeModificacion(std::string fecha) {
	this->fechaModificacion = fecha;
}


// Devuelve el nombre del archivo.
std::string Archivo::obtenerNombre() {
	return this->nombre;
}


// Devuelve el hash del contenido del archivo.
std::string Archivo::obtenerHash() {
	return this->hash;
}


// Devuelve la última fecha de modificación del archivo.
std::string Archivo::obtenerFechaDeModificacion() {
	return this->fechaModificacion;
}
