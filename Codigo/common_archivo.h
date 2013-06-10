//  
//  common_archivo.h
//  CLASE ARCHIVO
//  
//  Clase que representa a un archivo, guardando metadata de este
//  


#ifndef ARCHIVO_H
#define ARCHIVO_H


#include <string>
#include "common_convertir.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Archivo {
private:
	
	std::string nombre;					// Nombre del archivo
	std::string numBloque;				// Numero de bloque
	std::string bloque;					// Contenido del bloque en hexadecimal
	std::string hash;					// Hash del archivo
	std::string fechaModificacion;		// Fecha de la última modificación

public:

	// Constructor
	Archivo();

	Archivo(char* nombreDeArchivo);

	// Constructor
	Archivo(std::string nombreDeArchivo);

	// Constructor copia
	Archivo(const Archivo &a);

	// Destructor
	~Archivo();

	// Asigna un nombre al archivo
	void asignarNombre(std::string nombre);

	// Asigna un numero de bloque
	void asignarNumBloque(std::string numBloque);

	// Asigna el contenido del bloque
	void asignarBloque(std::string bloque);

	// Actualiza el hash del archivo
	void asignarHash(std::string hash);

	// Actualiza la fecha de la última modificación hecha sobre el archivo.
	void asignarFechaDeModificacion(std::string fecha);

	// Devuelve el nombre del archivo.
	std::string obtenerNombre();

	// Devuelve el numero de bloque del archivo
	std::string obtenerNumBloque();

	// Devuelve el contenido del bloque del archivo.
	std::string obtenerBloque();

	// Devuelve el hash del contenido del archivo.
	std::string obtenerHash();

	// Devuelve la última fecha de modificación del archivo.
	std::string obtenerFechaDeModificacion();

	/** Definicion de operadores **/
	// Operador asignacion
	void operator=(const Archivo& a);

	// Operador mayor: compara nombres de archivo en minuscula
	// Si son iguales, compara con nombres reales y como en la tabla ascii
	// (por ej 'a' > 'A')
	bool operator>(const Archivo& a); 

	// Operador menor: compara nombres de archivo en minuscula
	// Si son iguales, compara con nombres reales y como en la tabla ascii
	// (por ej 'a' > 'A')
	bool operator<(const Archivo& a);

	// Operador igual: compara nombres de archivo en minuscula 
	// Si son iguales, compara con nombres reales y como en la tabla ascii
	// (por ej 'a' > 'A')
	bool operator==(const Archivo& a);
};

#endif
