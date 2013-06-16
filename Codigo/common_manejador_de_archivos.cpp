//
//  common_manejador_de_archivos.h
//  CLASE MANEJADORDEARCHIVOS
//  


#include "common_manejador_de_archivos.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "dirent.h"




namespace {
	// Constantes para los nombres de directorio
	const std::string DIR_AU = ".au";

	// Constantes para los nombres de archivo
	const std::string ARCHIVO_REG_ARCHIVOS = ".reg_archivos";
}





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
ManejadorDeArchivos::ManejadorDeArchivos(const std::string& directorio) : 
	directorio(directorio) { }


// Destructor
ManejadorDeArchivos::~ManejadorDeArchivos() { }


// Devuelve una lista con los archivos (ordenados por nombre) que se 
// encuentran ubicados en el directorio administrado por el manejador.
void ManejadorDeArchivos::obtenerArchivosDeDirectorio(
	Lista<std::string>* listaArchivos) {
	// Variables auxiliares
	DIR *dir;
	struct dirent *entrada = 0;
	unsigned char esDirectorio =0x4;

	// Abrimos directorio y procesamos si fue exitosa la apertura
	if((dir = opendir (this->directorio.c_str())) != NULL) {
		// Iteramos sobre cada objeto del directorio
		while ((entrada = readdir (dir)) != NULL) {
			// Salteamos directorios
			if (entrada->d_type == esDirectorio)
				continue;

			// Insertamos el nombre de archivo en la lista
			listaArchivos->insertarUltimo(entrada->d_name);
		}

		closedir(dir);
	} 
	else 
		throw "ERROR: No se ha podido abrir el directorio.";

	// Ordenamos la lista de archivos
	listaArchivos->ordenar();
}


// Agrega un nuevo archivo al directorio.
// PRE: 'nombreArchivo' es el nombre del archivo nuevo; 'contenido' es el
// contenido del archivo nuevo expresado en formato hexadecimal como una
// cadena.
// POST: devuelve true si se agregó el archivo con éxito o false en caso
// contrario
bool ManejadorDeArchivos::agregarArchivo(const std::string& nombreArchivo, 
	const std::string& contenido) {

	return 0;
}


// Elimina un archivo del directorio.
// PRE: 'nombreArchivo' es el nombre de archivo.
// POST: devuelve true si se eliminó con éxito o false en su defecto.
bool ManejadorDeArchivos::eliminarArchivo(const std::string& nombreArchivo) {
	// Variables auxiliares
	std::fstream archivo;

	// Armamos ruta del archivo
	std::string ruta = this->directorio + "/" + nombreArchivo;

	// Busca el archivo y si lo encuentra, lo borra
	archivo.open(ruta.c_str(), std::ios_base::in);
	
	// Comprobamos si se abrió el archivo, señal de que existe
	if(!archivo.is_open()) return false;
	
	// Cerramos el archivo y lo eliminamos del directorio
	archivo.close();
	remove(ruta.c_str());

	return true;
}


// Devuelve el hash del archivo, el cual se encuentra conformado
// por los hashes de cada bloque concatenados.
// PRE: 'nombreArchivo' es el nombre de archivo, 'hashArchivo' es
// el string en donde se depositará el hash.
// POST: se devuelve la cantidad de bloques que posee actualmente el
// archivo.
int ManejadorDeArchivos::obtenerHash(const std::string& nombreArchivo, 
	std::string& hashArchivo) {

	hashArchivo = "UNHASH";
	return 0;
}


// Devuelve el contenido de un archivo en formato hexadecimal expresado
// en una cadena de caracteres.
// PRE: 'nombreArchivo' es el nombre de archivo; 'numBloque' es el
// número de bloque que se desea obtener el archivo. Si no se especifica
// o si es cero, se devuelve el contenido completo del archivo.
std::string ManejadorDeArchivos::obtenerContenido(
	const std::string& nombreArchivo, int numBloque) {

	return "";
}


// Modifica el contenido del bloque de un archivo.
// PRE: 'nombreArchivo' es el nombre de archivo; 'numBloque' es el
// numero de bloque que se desea modificar; 'contenido' es el contenido
// nuevo del bloque expresado en formato hexadecimal.
void ManejadorDeArchivos::modificarBloque(const std::string& nombreArchivo,
	const int numBloque, const std::string& contenido) {

}


// Compara el hash actual de cierto bloque de archivo con un hash pasado
// por parámetro.
// PRE: 'nombreArchivo' es el nombre de archivo; 'numBloque' es el
// número del bloque que se desea comparar; 'hash' es el hash que
// se comparará con el del bloque del archivo.
// POST: devuelve true si son iguales o false si presentan diferencias.
bool ManejadorDeArchivos::compararBloque(const std::string& nombreArchivo,
	const int numBloque, const std::string hash) {

	return false;
}




// DE AQUI EN ADELANTE CONSIDERAR MODIFICACIONES



// Crea un archivo de registro.
// POST: devuelve true si se realizó la creación con éxito o false en su
// defecto. Si ya se encuentra existente también devuelve false.
bool ManejadorDeArchivos::crearRegistroDeArchivos() {
	// Comprobamos si existia previamente el archivo
	if(this->existeRegistroDeArchivos()) return false;

	// Armamos ruta del registro
	std::string registro = this->directorio + DIR_AU + "/" 
		+ ARCHIVO_REG_ARCHIVOS;

	// Creamos la carpeta que contiene los registros
	mkdir((this->directorio + DIR_AU).c_str(), S_IRWXU | S_IRWXG | S_IROTH |
		S_IXOTH);

	// Creamos el registro
	std::ofstream archivo;
	archivo.open(registro.c_str(), std::ios::out);

	return true;
}


// Actualiza el registro local de archivos.
// PRE: 'nuevos', 'modificados' y 'eliminados' son punteros a cola donde
// se insertarán los nombres de archivo correspondientes a la situación
// en la que se encuentren.
// POST: se devuelve 'false' si se produjeron cambios en el registro o
// 'true' en su defecto; esto evita tener que revisar las colas para
// comprobar cambios.
bool ManejadorDeArchivos::actualizarRegistroDeArchivos(
	Cola< std::string > *nuevos, Cola< std::string > *modificados, 
	Cola< std::string > *eliminados) {

	return false;
}


// Actualiza el registro local de archivos.
// POST: se devuelve 'false' si se produjeron cambios en el registro o
// 'true' en su defecto; esto evita tener que revisar las colas para
// comprobar cambios.
bool ManejadorDeArchivos::actualizarRegistroDeArchivos() {

	return false;
}


// Comprueba si existe el registro de archivos.
// POST: devuelve true si existe o false en su defecto.
bool ManejadorDeArchivos::existeRegistroDeArchivos() {
	// Armamos ruta del registro
	std::string registro = this->directorio + DIR_AU + "/" 
		+ ARCHIVO_REG_ARCHIVOS;

	// Tratamos de abrir el archivo
	std::ifstream archivo;
	archivo.open(registro.c_str());

	// Si no se abrió retornamos false
	if(!archivo.good()) return false;

	// Si se abrió, lo cerramos y retornamos true
	archivo.close();
	return true;
}

