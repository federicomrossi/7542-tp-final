//  
//  client_manejador_archivos.h
//  CLASE MANEJADORARCHIVOS
//  



#include "client_manejador_archivos.h"
#include "common_convertir.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>



/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
ManejadorArchivos::ManejadorArchivos() { }


//
std::string ManejadorArchivos::obtenerArchivo(
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
