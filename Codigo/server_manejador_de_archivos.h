// Manejador de archivos 

#ifndef MANEJADOR_DE_ARCHIVOS_H
#define MANEJADOR_DE_ARCHIVOS_H

#include "common_mutex.h"
#include "common_lock.h"
#include "common_hash.h"
#include <string>
#include <stdio.h>
#include <list>
#include <iostream>
#include <fstream>
#include <utility>




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ManejadorDeArchivos {
private:

	std::string directorio;				// Directorio sobre el cual se trabaja
	Mutex m;							// Mutex

public:

	// Constructor
	ManejadorDeArchivos(const std::string& directorio);

	// Destructor
	~ManejadorDeArchivos();

	// Elimina un archivo o un bloque de un archivo del directorio local
	// Devuelve 0 en caso de eliminar correctamente el archivo
	// Por ahora solamente borra archivos enteros, num_bloque = WHOLE_FILE
	int eliminarArchivo(const std::string &nombre_archivo, 
		const std::string &num_bloque);

	// Agrega un nuevo archivo o un bloque al directorio 
	// Por ahora solamente guarda archivos enteros, num_bloque = WHOLE_FILE
	// Devuelve el hash del archivo
	int agregarArchivo(const std::string &nombre_archivo, 
		const std::string &num_bloque, const std::string &bloque_archivo, std::string &hash);

	// Devuelve una lista con los nombre de archivos (ordenados 
	// alfabeticamente) que se encuentran ubicados en el directorio 
	// administrado por el manejador.
	std::list< std::pair<std::string, std::string> > obtenerArchivosDeDirectorio();
};

#endif /* MANEJADOR_DE_ARCHIVOS_H */
