//
//  common_manejador_de_archivos.h
//  CLASE MANEJADORDEARCHIVOS
//  


#ifndef MANEJADOR_DE_ARCHIVOS_H
#define MANEJADOR_DE_ARCHIVOS_H

#include "common_mutex.h"
#include "common_lock.h"
#include "common_hash.h"
#include "common_lista.h"
#include "common_cola.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>



namespace {
	// Constante que define el tamaño de los bloques de archivos.
	const int TAMANIO_BLOQUE = 8;
}




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

	// Devuelve una lista con los archivos (ordenados por nombre) que se 
	// encuentran ubicados en el directorio administrado por el manejador.
	void obtenerArchivosDeDirectorio(Lista<std::string>* listaArchivos);

	// Agrega un nuevo archivo al directorio.
	// PRE: 'nombreArchivo' es el nombre del archivo nuevo; 'contenido' es el
	// contenido del archivo nuevo expresado en formato hexadecimal como una
	// cadena.
	// POST: devuelve true si se agregó el archivo con éxito o false en caso
	// contrario
	bool agregarArchivo(const std::string& nombreArchivo, 
		const std::string& contenido);

	// Elimina un archivo del directorio.
	// PRE: 'nombreArchivo' es el nombre de archivo.
	// POST: devuelve true si se eliminó con éxito o false en su defecto.
	bool eliminarArchivo(const std::string& nombreArchivo);

	// Devuelve el hash del archivo, el cual se encuentra conformado
	// por los hashes de cada bloque concatenados.
	// PRE: 'nombreArchivo' es el nombre de archivo, 'hashArchivo' es
	// el string en donde se depositará el hash.
	// POST: se devuelve la cantidad de bloques que posee actualmente el
	// archivo.
	int obtenerHash(const std::string& nombreArchivo, 
		std::string& hashArchivo);

	// Devuelve el contenido de un archivo en formato hexadecimal expresado
	// en una cadena de caracteres.
	// PRE: 'nombreArchivo' es el nombre de archivo; 'numBloque' es el
	// número de bloque que se desea obtener el archivo. Si no se especifica
	// o si es cero, se devuelve el contenido completo del archivo.
	// POST: devuelve una cadena que representa el contenido.
	std::string obtenerContenido(const std::string& nombreArchivo,
		int numBloque = 0);

	// Modifica el contenido del bloque de un archivo.
	// PRE: 'nombreArchivo' es el nombre de archivo; 'numBloque' es el
	// numero de bloque que se desea modificar; 'contenido' es el contenido
	// nuevo del bloque expresado en formato hexadecimal.
	void modificarBloque(const std::string& nombreArchivo, const int numBloque,
		const std::string& contenido);

	// Compara el hash actual de cierto bloque de archivo con un hash pasado
	// por parámetro.
	// PRE: 'nombreArchivo' es el nombre de archivo; 'numBloque' es el
	// número del bloque que se desea comparar; 'hash' es el hash que
	// se comparará con el del bloque del archivo.
	// POST: devuelve true si son iguales o false si presentan diferencias.
	bool compararBloque(const std::string& nombreArchivo, const int numBloque,
		const std::string hash);





	// FALTAN AGREGAR MÉTODOS DE CREACIÓN Y ACTUALIZACIÓN DEL REGISTRO 
	// DE ARCHIVOS. LAS FIRMAS SIGUIENTES ESTAN SUJETAS A MODIFICACIONES
	////////////////////////////////////////////////////////////////////


	// Crea un archivo de registro.
	// PRE: 'nombreArchivo' es la ruta hacia el archivo junto a su nombre.
	// POST: devuelve true si se realizó la creación con éxito o false en su
	// defecto.
	bool crearRegistroDeArchivos();

	// Actualiza el registro local de archivos.
	// PRE: 'nuevos', 'modificados' y 'eliminados' son punteros a cola donde
	// se insertarán los nombres de archivo correspondientes a la situación
	// en la que se encuentren.
	// POST: se devuelve 'false' si se produjeron cambios en el registro o
	// 'true' en su defecto; esto evita tener que revisar las colas para
	// comprobar cambios.
	bool actualizarRegistroDeArchivos(Cola< std::string > *nuevos, 
		Cola< std::string > *modificados, Cola< std::string > *eliminados);

	// Actualiza el registro local de archivos.
	// POST: se devuelve 'false' si se produjeron cambios en el registro o
	// 'true' en su defecto; esto evita tener que revisar las colas para
	// comprobar cambios.
	bool actualizarRegistroDeArchivos();

	// Comprueba si existe cierto registro de archivos.
	// PRE: 'nombreArchivo' es la ruta hacia el archivo junto a su nombre.
	// POST: devuelve true si existe o false en su defecto.
	bool existeRegistroDeArchivos();
};

#endif
