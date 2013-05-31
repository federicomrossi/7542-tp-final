//  
//  client_manejador_de_archivos.cpp
//  CLASE MANEJADORDEARCHIVOS
//  


#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "dirent.h"
#include "common_convertir.h"
#include "common_hash.h"
#include "client_manejador_de_archivos.h"


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


// Devuelve el contenido de un archivo en formato hexadecimal expresado
// en una cadena de caracteres
std::string ManejadorDeArchivos::obtenerContenidoArchivo(
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
	// Variables auxiliares
	std::ifstream registro;
	std::ofstream registroTmp;
	bool huboCambio = false;

	// Armamos rutas de archivos
	std::string regNombre = this->directorio + "/" + DIR_AU + "/" 
		+ ARCHIVO_REG_ARCHIVOS;
	std::string regTmpNombre = this->directorio + "/" + DIR_AU + "/" 
		+ ARCHIVO_REG_ARCHIVOS + "~";

	// Abrimos el registro original y el registro temporal
	registro.open(regNombre.c_str(), std::ios::in);
	registroTmp.open(regTmpNombre.c_str(), std::ios::app);

	// Verificamos si la apertura fue exitosa
	if(!registro.is_open() || !registroTmp.is_open()) 
		throw "ERROR: El registro no pudo ser abierto.";

	// Relevamos los nombres de archivos ubicados actualmente en el directorio
	std::list<std::string> l = this->obtenerArchivosDeDirectorio();
	std::list< std::string>::iterator it_archivoNombre;
	std::string nombreArchivo_reg, hashArchivo_reg;


	// Iteramos sobre los nombres de archivos existentes en el directorio
	for (it_archivoNombre = l.begin(); it_archivoNombre != l.end();
		++it_archivoNombre) {

		// Tomamos un registro
		registro >> nombreArchivo_reg >> hashArchivo_reg;

		// Si no hay mas registros, insertamos los archivos faltantes al
		// final
		if(registro.eof()) {
			registroTmp << *it_archivoNombre << " " << 
					Hash::funcionDeHash(*it_archivoNombre) << std::endl;

			huboCambio = true;
			continue;
		}

		// Comprobamos si se han eliminado archivos, salteando a estos mismos
		while(*it_archivoNombre > nombreArchivo_reg) {
			registro >> nombreArchivo_reg >> hashArchivo_reg;
			huboCambio = true;
		}

		// Caso en que los nombres coinciden
		if(nombreArchivo_reg == *it_archivoNombre) {
			// Caso en que el hash coincide
			if(hashArchivo_reg == Hash::funcionDeHash(*it_archivoNombre))
				registroTmp << nombreArchivo_reg << " " << hashArchivo_reg 
					<< std::endl;
			// Caso en que el hash no coincide
			else
			{
				registroTmp << *it_archivoNombre << " " << 
					Hash::funcionDeHash(*it_archivoNombre) << std::endl;
				
				huboCambio = true;
			}
		}
		// Caso en que se ha agregado un archivo nuevo al directorio
		else if(*it_archivoNombre < nombreArchivo_reg) {
			// Insertamos el nuevo registro
			registroTmp << *it_archivoNombre << " " << 
				Hash::funcionDeHash(*it_archivoNombre) << std::endl;

			huboCambio = true;
		}
	}

	// Cerramos archivos
	registro.close();
    registroTmp.close();

    // Eliminamos el registro original y convertimos el temporal en el oficial
	remove(regNombre.c_str());
	rename(regTmpNombre.c_str(), regNombre.c_str());

	return huboCambio;
}




/*
 *  IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */


// Devuelve una lista con los nombre de archivos (ordenados 
// alfabeticamente) que se encuentran ubicados en el directorio 
// administrado por el manejador.
std::list<std::string> ManejadorDeArchivos::obtenerArchivosDeDirectorio() {
	// Variables auxiliares
	DIR *dir;
	struct dirent *entrada = 0;
	std::list<std::string> listaArchivos;
	unsigned char esDirectorio =0x4;

	// Abrimos directorio y procesamos si fue exitosa la apertura
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

