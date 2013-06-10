//  
//  common_archivo.h
//  CLASE ARCHIVO
//  
//  Clase que representa a un archivo, guardando metadata de este
//  


#ifndef ARCHIVO_H
#define ARCHIVO_H


#include <string>





/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Archivo {
private:
	
	std::string nombre;					// Nombre del archivo
	std::string hash;					// Hash del archivo
	std::string fechaModificacion;		// Fecha de la última modificación

public:

	// Constructor
	Archivo(std::string nombreDeArchivo);
	
	// Destructor
	~Archivo();

	// Actualiza el hash del archivo
	void asignarHash(std::string hash);

	// Actualiza la fecha de la última modificación hecha sobre el archivo.
	void asignarFechaDeModificacion(std::string fecha);

	// Devuelve el nombre del archivo.
	std::string obtenerNombre();

	// Devuelve el hash del contenido del archivo.
	std::string obtenerHash();

	// Devuelve la última fecha de modificación del archivo.
	std::string obtenerFechaDeModificacion();
};

#endif
