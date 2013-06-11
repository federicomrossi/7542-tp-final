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
#include "common_lista.h"
#include "common_archivo.h"

// DEBUG: Debe buscarse otra lugar para la constante WHOLE_FILE
#include "common_protocolo.h"

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


	// Devuelve una lista con los nombre de archivos (ordenados 
	// alfabeticamente) que se encuentran ubicados en el directorio 
	// administrado por el manejador.
	std::list<std::string> obtenerArchivosDeDirectorio(); 

	// Devuelve una lista con los archivos (ordenados por nombre) que se encuentran 
	// ubicados en el directorio administrado por el manejador.
	void obtenerArchivosDeDirectorio(Lista<Archivo>* listaArchivos);

public:

	// Constructor
	ManejadorDeArchivos(const std::string& directorio);

	// Destructor
	~ManejadorDeArchivos();

	// Devuelve el contenido de un archivo en formato hexadecimal expresado
	// en una cadena de caracteres
	std::string obtenerContenidoArchivo(const std::string& nombre_archivo);

	// Devuelve un archivo que se localiza en el directorio
	// De no existir, devuelve un codigo de error = 0
	bool obtenerArchivo(const std::string &nombre_archivo, Archivo& archivo);

	// Actualiza el registro local de archivos.
	// PRE: 'nuevos', 'modificados' y 'eliminados' son punteros a cola donde
	// se insertarán los nombres de archivo correspondientes a la situación
	// en la que se encuentren.
	// POST: se devuelve 'false' si se produjeron cambios en el registro o
	// 'true' en su defecto; esto evita tener que revisar las colas para
	// comprobar cambios.
	bool actualizarRegistroDeArchivos(Cola< std::string > *nuevos, 
		Cola< std::string > *modificados, Cola< std::string > *eliminados);

	// Recibe una lista de archivos, compara con la que se encuentra localmente 
	// * Lista: lista de archivos con la cual se compara
	// * Faltantes: lista de archivos que no estan en el dir local
	// * Sobrantes: lista de archivos que no estan en la lista
	void obtenerListaDeActualizacion(Lista<Archivo>* listaExterna, Lista<Archivo>* faltantes,
		Lista<Archivo>* sobrantes);

	// Elimina un archivo o un bloque de un archivo del directorio local
	// Devuelve 0 en caso de eliminar correctamente el archivo
	// Por ahora solamente borra archivos enteros, num_bloque = WHOLE_FILE
	int eliminarArchivo(const std::string &nombre_archivo, 
		const std::string &num_bloque);

	// Agrega un nuevo archivo o un bloque al directorio 
	// Por ahora solamente guarda archivos enteros, num_bloque = WHOLE_FILE
	int agregarArchivo(const std::string &nombre_archivo, 
		const std::string &num_bloque, const std::string &bloque_archivo);

	// Devuelve el hash del archivo con nombre especificado por parametros
	// De no existir, se devuelve un string vacio
	std::string obtenerHashArchivo(const std::string &nombre_archivo);
};

#endif
