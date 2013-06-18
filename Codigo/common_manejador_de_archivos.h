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
#include <utility>
#include <math.h>




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

	// Separa de una linea el nombre y el hash
	void separarNombreYHash(const std::string &linea, std::string& nombre, std::string &hash);

	// Devuelve un string de caracteres alfanumericos aleatorios de 
	// tamanio 'longitud'
	void randomString(std::string &s, int longitud);

public:

	// Constructor
	ManejadorDeArchivos(const std::string& directorio);

	// Destructor
	~ManejadorDeArchivos();

	// Devuelve una lista con los archivos (ordenados por nombre) que se 
	// encuentran ubicados en el directorio administrado por el manejador.
	void obtenerArchivosDeDirectorio(Lista<std::string>* listaArchivos);

	// Devuelve una lista con los archivos (ordenados por nombre) que se 
	// encuentran ubicados en el registro administrado por el manejador.
	void obtenerArchivosDeRegistro(Lista< std::pair<std::string, 
		std::string> >* listaArchivos);

	// Agrega un nuevo archivo al directorio.
	// PRE: 'nombreArchivo' es el nombre del archivo nuevo; 'contenido' es el
	// contenido del archivo nuevo expresado en formato hexadecimal como una
	// cadena.
	// Tira una excepcion si no logra crear un archivo nuevo
	void agregarArchivo(const std::string& nombreArchivo, 
		const std::string& contenido);

	// Elimina un archivo del directorio.
	// PRE: 'nombreArchivo' es el nombre de archivo.
	// POST: devuelve true si se eliminó con éxito o false en su defecto.
	bool eliminarArchivo(const std::string& nombreArchivo);

	// Guarda modificaciones de bloques en un archivo, dejandolo de menor o 
	// mayor tamanio segun corresponda
	void modificarArchivo(std::string& nombreArchivo, 
		int cantBloquesDelArchivo, 
		Lista< std::pair< int, std::string > >& listaBloquesAReemplazar);

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

	// Recibe una lista de archivos, compara con la que se encuentra 
	// localmente.
	// * ListaExterna: lista de archivos con la cual se compara
	// * Faltantes: lista de archivos que no estan en el dir local
	// * Sobrantes: lista de archivos que no estan en la lista que se deben
	// eliminar del dir local
	void obtenerListaDeActualizacion(Lista< std::pair< std::string, 
		std::pair< std::string, int > > >* listaExterna,
		Lista< std::pair< std::string, Lista<int> > >* faltantes, 
		Lista<std::string>* sobrantes);

	// Devuelve las diferencias que existen entre 2 archivos
	void obtenerListaDiferencias(std::string nombre, int cantBloques, 
		Lista<int>* diferencias);

	// Devuelve la cantidad de bloques de un archivo
	int obtenerCantBloques(const std::string &nombreArchivo);

	// Procesa dos hashes pertenecientes al contenido de un archivo y
	// obtiene los bloques que han cambiado.
	// PRE: 'hashViejo' y 'hashNuevo' son los hashes de los archivos a
	// procesar; 'cantNuevaBloques' es la cantidad de bloques del archivo
	// que es representado por 'hashNuevo'
	// POST: se listan en 'listaBLoquesDiferentes' los numero de bloques
	// que han cambiado; Se devuelve true si se encontraron diferencias o 
	// false en caso contrario.
	bool obtenerDiferencias(std::string hashViejo, std::string hashNuevo,
		int& cantNuevaBloques, Lista<int> *listaBloquesDiferentes);

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
