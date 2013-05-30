//  
//  client_manejador_archivos.h
//  CLASE MANEJADORARCHIVOS
//  



#include "client_manejador_archivos.h"
#include "common_convertir.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "dirent.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
ManejadorArchivos::ManejadorArchivos(const std::string& directorio) : 
	directorio(directorio) { }


// Destructor
ManejadorArchivos::~ManejadorArchivos() { }


// Devuelve el contenido de un archivo en formato hexadecimal expresado
// en una cadena de caracteres
std::string ManejadorArchivos::obtenerContenidoArchivo(
	const std::string& nombre_archivo) {
	// Abrimos el archivo
	std::ifstream archivo(nombre_archivo.c_str(), 
		std::ios::in | std::ios::binary | std::ios::ate);

	if(!archivo.is_open())
		throw "ERROR: Archivo de entrada inválido.";

	std::ifstream::pos_type size;
	uint8_t * contenidoTemp;

	// Almacenamos momentaneamente el contenido del archivo original
	size = archivo.tellg();
	contenidoTemp = new uint8_t[size];
	archivo.seekg(0, std::ios::beg);
	archivo.read((char*)contenidoTemp, size);
	archivo.close();

	// Convertimos el contenido a hexadecimal
	std::string contenidoHex(Convertir::uitoh(contenidoTemp, size));
	delete[] contenidoTemp;

	return contenidoHex;
}


// Devuelve una lista con los nombre de archivos que se encuentran ubicados
// en el directorio que se encuentra administrando el manejador.
std::list<std::string> ManejadorArchivos::obtenerArchivosDeDirectorio()
{
	DIR *dir;
	struct dirent *entrada = 0;
	std::list<std::string> listaArchivos;
	unsigned char esDirectorio =0x4;

	if((dir = opendir (this->directorio.c_str())) != NULL) {
		// Iteramos sobre cada objeto del directorio
		while ((entrada = readdir (dir)) != NULL) {
			// Salteamos directorios
			if (entrada->d_type == esDirectorio)
				continue;

			// Insertamos el nombre de archivo en la lista
			listaArchivos.push_back(entrada->d_name);
		}

		closedir(dir);
	} 
	else 
		throw "ERROR: No se ha podido abrir el directorio.";

	listaArchivos.sort();
	return listaArchivos;
}









