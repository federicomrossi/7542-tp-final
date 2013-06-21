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

// Operador asignacion
void Archivo::operator=(const Archivo& a) {
	this->nombre = a.nombre;
	this->hash = a.hash;
	this->fechaModificacion = a.fechaModificacion;
}

// Operador mayor: compara nombres de archivo en minuscula
// Si son iguales, compara con nombres reales y como en la tabla ascii
// (por ej 'a' > 'A')
bool Archivo::operator>(const Archivo& a) {
	std::string nombreMinuscula = Convertir::toLowercase(this->nombre);
	std::string nombreAComparar = Convertir::toLowercase(a.nombre);

	bool esMayor;

	if (nombreMinuscula == nombreAComparar)  // Analizo el caso real entonces
		esMayor = (this->nombre > a.nombre);
	else 	
		esMayor = (nombreMinuscula > nombreAComparar);

	return esMayor;
}

// Operador menor: compara nombres de archivo en minuscula
// Si son iguales, compara con nombres reales y como en la tabla ascii
// (por ej 'a' > 'A')
bool Archivo::operator<(const Archivo& a) {
	std::string nombreMinuscula = Convertir::toLowercase(this->nombre);
	std::string nombreAComparar = Convertir::toLowercase(a.nombre);

	bool esMenor;

	if (nombreMinuscula == nombreAComparar)  // Analizo el caso real entonces
		esMenor = (this->nombre < a.nombre);
	else {
		esMenor = (nombreMinuscula < nombreAComparar);
	}

	return esMenor;
}

// Operador igual: compara nombres de archivo en minuscula 
// Si son iguales, compara con nombres reales y como en la tabla ascii
// (por ej 'a' > 'A')
bool Archivo::operator==(const Archivo& a) {
	std::string nombreMinuscula = Convertir::toLowercase(this->nombre);
	std::string nombreAComparar = Convertir::toLowercase(a.nombre);

	bool esIgual = false;

	if (nombreMinuscula == nombreAComparar) {
		if (this->nombre == a.nombre)
			esIgual = true;
	}
	
	return esIgual;
}
