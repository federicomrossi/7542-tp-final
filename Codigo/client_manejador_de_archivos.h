//
//  client_manejador_de_archivos.h
//  CLASE MANEJADORDEARCHIVOS
//  


#ifndef MANEJADOR_DE_ARCHIVOS_H
#define MANEJADOR_DE_ARCHIVOS_H


#include <string>
#include <list>
#include <stdio.h>
#include "common_mutex.h"
#include "common_lock.h"
#include "common_cola.h"

//DEBUG
#include <iostream>
//END DEBUG



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ManejadorDeArchivos {
private:

	std::string directorio;				// Directorio sobre el cual se trabaja
	Mutex m;							// Mutex
	bool inicializoRegistro;			// Flag que sensa si se inicializó el
										// registro de archivos.


	// Devuelve una lista con los nombre de archivos (ordenados 
	// alfabeticamente) que se encuentran ubicados en el directorio 
	// administrado por el manejador.
	std::list<std::string> obtenerArchivosDeDirectorio(); 

public:

	// Constructor
	ManejadorDeArchivos(const std::string& directorio);

	// Destructor
	~ManejadorDeArchivos();

	// 
	void inicializarRegistroDeArchivos(std::string listaDeArchivos);

	// Comprueba si se inicializó el registro de archivos.
	// POST: devuelve true si se inicializó o false en caso contrario.
	bool seInicializoRegistroDeArchivos();

	// Devuelve el contenido de un archivo en formato hexadecimal expresado
	// en una cadena de caracteres
	std::string obtenerContenidoArchivo(const std::string& nombre_archivo);

	// Actualiza el registro local de archivos.
	// PRE: 'nuevos', 'modificados' y 'eliminados' son punteros a cola donde
	// se insertarán los nombres de archivo correspondientes a la situación
	// en la que se encuentren.
	// POST: se devuelve 'false' si se produjeron cambios en el registro o
	// 'true' en su defecto; esto evita tener que revisar las colas para
	// comprobar cambios.
	bool actualizarRegistroDeArchivos(Cola< std::string > *nuevos, 
		Cola< std::string > *modificados, Cola< std::string > *eliminados);

	// Elimina un archivo o un bloque de un archivo del directorio local
	// Devuelve 0 en caso de eliminar correctamente el archivo
	// Por ahora solamente borra archivos enteros, num_bloque = WHOLE_FILE
	int eliminarArchivo(const std::string &nombre_archivo, 
		const std::string &num_bloque);

	// Agrega un nuevo archivo o un bloque al directorio 
	// Por ahora solamente guarda archivos enteros, num_bloque = WHOLE_FILE
	int agregarArchivo(const std::string &nombre_archivo, 
		const std::string &num_bloque, const std::string &bloque_archivo);
};

#endif
