//  
//  common_archivo.cpp
//  CLASE ARCHIVO
//  
//  Clase que representa a un archivo, guardando metadata de este
//  


#include "common_archivo.h"


// Constructor
Archivo::Archivo() { }


Archivo::Archivo(char* nombreDeArchivo) {
	nombre.assign(nombreDeArchivo);
}

// Constructor
Archivo::Archivo(std::string nombreDeArchivo) : nombre(nombreDeArchivo) { }


// Constructor copia
Archivo::Archivo(const Archivo &a) : nombre(a.nombre), hash(a.hash), 
	fechaModificacion(a.fechaModificacion) { }


// Destructor
Archivo::~Archivo() { }


// Asigna un nombre al archivo
void Archivo::asignarNombre(std::string nombre) {
	this->nombre = nombre;
}


// Asigna un numero de bloque
void Archivo::asignarNumBloque(std::string numBloque) {
	this->numBloque = numBloque;
}


// Asigna el contenido del bloque
void Archivo::asignarBloque(std::string bloque) {
	this->bloque = bloque;
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


// Devuelve el numero de bloque del archivo
std::string Archivo::obtenerNumBloque() {
	return this->numBloque;
}


// Devuelve el contenido del bloque del archivo.
std::string Archivo::obtenerBloque() {
	return this->bloque;
}



// Devuelve el hash del contenido del archivo.
std::string Archivo::obtenerHash() {
	return this->hash;
}


// Devuelve la última fecha de modificación del archivo.
std::string Archivo::obtenerFechaDeModificacion() {
	return this->fechaModificacion;
}

Archivo Archivo::operator=(Archivo a) {
	Archivo b;
	b.nombre = a.nombre;
	b.hash = a.hash;
	b.fechaModificacion = a.fechaModificacion;
	return b;
}

