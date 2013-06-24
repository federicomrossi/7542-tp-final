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
#include "common_logger.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <utility>
#include <math.h>





/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ManejadorDeArchivos {
private:

	std::string directorio;				// Directorio sobre el cual se trabaja
	Mutex mArc;							// Mutex para accesos fisicos
	Mutex mReg;							// Mutex para accesos sobre registros
	Logger *logger;						// Logger de eventos



	// Procesa dos hashes pertenecientes al contenido de un archivo y
	// obtiene los bloques que han cambiado.
	// PRE: 'hashViejo' y 'hashNuevo' son los hashes de los archivos a
	// procesar; 'cantNuevaBloques' es la cantidad de bloques del archivo
	// que es representado por 'hashNuevo'
	// POST: se listan en 'listaBLoquesDiferentes' los numero de bloques
	// que han cambiado; Se devuelve true si se encontraron diferencias o 
	// false en caso contrario.
	bool obtenerDiferencias(std::string& hashViejo, std::string& hashNuevo,
		int& cantNuevaBloques, Lista<int> *listaBloquesDiferentes);

	// Separa de una linea el nombre y el hash
	void separarNombreYHash(const std::string &linea, std::string& nombre, std::string &hash);

public:

	// Constructor
	ManejadorDeArchivos(const std::string& directorio, Logger *logger);

	// Destructor
	~ManejadorDeArchivos();

	// Devuelve una lista con los archivos (ordenados por nombre) que se 
	// encuentran ubicados en el directorio administrado por el manejador.
	void obtenerArchivosDeDirectorio(Lista<std::string>* listaArchivos);

	// Devuelve una lista con los archivos (ordenados por nombre) que se 
	// encuentran ubicados en el registro administrado por el manejador.
	void obtenerArchivosDeRegistro(Lista<std::string>* listaArchivos,
		Lista< std::pair< std::string, std::string> >* listaPares);

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

	// Realiza modificaciones sobre los bloques de un archivo.
	// PRE: 'nombreArchivo' es el nombre del archivo a modificar;
	// 'cantloquesDelArchivo' es la cantidad nueva de bloques que debe
	// contener el archivo; 'listaBloquesAReemplazar' es una lista que
	// contiene los números de bloque y su respectivo contenido, los
	// cuales reemplazarán a los bloques actuales.
	void modificarArchivo(std::string& nombreArchivo, 
		unsigned int cantBytesDelArchivo, 
		Lista< std::pair< int, std::string > >& listaBloquesAReemplazar);

	// Comprueba la existencia de un archivo en el directorio.
	// PRE: 'nombreArchivo' es el nombre de archivo a buscar.
	// POST: devuelve true si existe o false en caso contrario.
	bool existeArchivo(std::string& nombreArchivo);

	// Calcula el hash del archivo, el cual se encuentra conformado
	// por los hashes de cada bloque concatenados.
	// PRE: 'nombreArchivo' es el nombre de archivo, 'hashArchivo' es
	// el string en donde se depositará el hash.
	// POST: se devuelve la cantidad de bloques que posee actualmente el
	// archivo.
	int obtenerHash(const std::string& nombreArchivo, 
		std::string& hashArchivo);

	// Devuelve el hash del archivo, el cual se encuentra conformado
	// por los hashes de cada bloque concatenados.
	// PRE: 'nombreArchivo' es el nombre de archivo de registro, 
	// 'listaNombreHashReg' es una lista de pares de <nombre,hash> del 
	// archivo registro
	// POST: Se devuelve el hash
	std::string obtenerHashRegistro(Lista< std::pair< std::string, 
		std::string> > * listaNombreHashReg, std::string& nombreArchivo);

	// Devuelve el hash del bloque de un archivo.
	// PRE: 'nombreArchivo' es el nombre de archivo del bloque; 'numBloque'
	// es el número de bloque del que se desea obtener el hash.
	// POST: se devuelve una cadena con el hash del bloque.
	std::string obtenerHashDeBloque(const std::string& nombreArchivo,
		int numBloque);

	// Devuelve el contenido de un archivo en formato hexadecimal expresado
	// en una cadena de caracteres.
	// PRE: 'nombreArchivo' es el nombre de archivo; 'numBloque' es el
	// número de bloque que se desea obtener el archivo. Si no se especifica
	// o si es cero, se devuelve el contenido completo del archivo.
	// POST: devuelve una cadena que representa el contenido.
	std::string obtenerContenido(const std::string& nombreArchivo,
		int numBloque = 0);

	// Recibe una lista de archivos, compara con la que se encuentra localmente 
	// * ListaExterna: lista de archivos con la cual se compara
	// * Faltantes: lista de archivos que estan modificados en el dir local
	// * Sobrantes: lista de archivos que no estan en la lista que se deben 
	// eliminar del dir local
	// * Nuevos: lista de archivos que no estan en el dir local
	void obtenerListaDeActualizacion(Lista< std::pair< std::string, 
		std::pair< std::string, int > > >* listaExterna,
		Lista< std::pair< std::string, Lista<int> > >* faltantes, 
		Lista<std::string>* sobrantes);

	// Devuelve la cantidad de bloques de un archivo
	int obtenerCantBloques(const std::string &nombreArchivo);

	// Devuelve la cantidad de Bytes de un archivo
	// PRE: 'nombreArchivo' es el nombre de archivo. El archivo no debe
	// sobrepasar los 4Gb de tamaño.
	// POST: en caso de no poder abrir el archivo (a causa de no existencia),
	// se devuelve 0. Se recomienda al usuario verificar la existencia
	// previamente para no confundir el cero de error con el valor nulo de
	// que puede poseer cierto archivo.
	unsigned int obtenerCantBytes(const std::string &nombreArchivo);

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
	bool actualizarRegistroDeArchivos(Cola< std::pair< std::string, 
		std::string > > *nuevos, Cola< std::pair< std::string, Lista<int> > > 
		*modificados, Cola< std::string > *eliminados);

	// Actualiza el registro local de archivos.
	// PRE: las listas corresponden a que archivos nuevos o modificados deben
	// tenerse en cuenta, siendo que los demás detectados en el momento de la
	// actualización, son salteados.
	// POST: se devuelve 'false' si se produjeron cambios en el registro o
	// 'true' en su defecto; esto evita tener que revisar las colas para
	// comprobar cambios.
	bool actualizarRegistroDeArchivos(Lista< std::string >& 
		nuevosActualizables, Lista< std::string >& modificadosActualizables);

	// Elimina el registro que identifica a un archivo en el registro de
	// archivos.
	// PRE: 'nombreArchivo' es el nombre del archivo a eliminar del registro.
	void borrarDeRegistroDeArchivos(const std::string& nombreArchivo);

	// Comprueba si existe cierto registro de archivos.
	// PRE: 'nombreArchivo' es la ruta hacia el archivo junto a su nombre.
	// POST: devuelve true si existe o false en su defecto.
	bool existeRegistroDeArchivos();

	// Corrobora si se encuentra registrado un archivo en el registro de
	// archivos.
	// PRE: 'nombreArchivo' es el nombre de archivo.
	// POST: devuelve true si el archivo se encuentra registrado o false
	// en caso contrario.
	bool existeArchivoEnRegitro(const std::string nombreArchivo);

	// Compara el hash actual de cierto bloque de archivo con un hash pasado
	// por parámetro.
	// PRE: 'nombreArchivo' es el nombre de archivo; 'numBloque' es el
	// número del bloque que se desea comparar; 'hash' es el hash que
	// se comparará con el del bloque del archivo.
	// POST: devuelve true si son iguales o false si presentan diferencias.
	bool compararBloque(const std::string& nombreArchivo, const int numBloque,
		const std::string& hash);
};

#endif
