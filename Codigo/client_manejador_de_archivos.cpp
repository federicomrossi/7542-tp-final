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
	// Bloqueamos el mutex
	Lock l(m);

	// Armamos la ruta hacia el archivo
	std::string ruta = directorio + "/" + nombre_archivo;

	// Abrimos el archivo
	std::ifstream archivo(ruta.c_str(), 
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
	// Bloqueamos el mutex
	Lock l(m);

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
	std::list<std::string> ld = this->obtenerArchivosDeDirectorio();
	std::list< std::string>::iterator it_archivoNombre;
	std::string reg_archivoNombre, reg_archivoHash;


	// Tomamos el primer registro
	registro >> reg_archivoNombre >> reg_archivoHash;

	// Iteramos sobre los nombres de archivos existentes en el directorio
	for (it_archivoNombre = ld.begin(); it_archivoNombre != ld.end();
		++it_archivoNombre) {

		// Caso en el que no hay mas registros y se han agregado archivos
		if(registro.eof()) {
			// Registramos archivo nuevo
			registroTmp << *it_archivoNombre << " " << 
				Hash::funcionDeHash(*it_archivoNombre) << std::endl;

			// Insertamos archivo en cola de nuevos
			nuevos->push(*it_archivoNombre);

			huboCambio = true;
			continue;
		}

		// Caso en que se han eliminado archivos
		while(*it_archivoNombre > reg_archivoNombre && !registro.eof()) {
			// Insertamos en cola de eliminados
			eliminados->push(reg_archivoNombre);

			// Tomamos el registro siguiente
			registro >> reg_archivoNombre >> reg_archivoHash;

			huboCambio = true;
		}

		// Caso en el que el archivo se mantiene existente
		if(*it_archivoNombre == reg_archivoNombre) {
			// Corroboramos si ha sido modificado
			if(reg_archivoHash != Hash::funcionDeHash(*it_archivoNombre)) {
				// Actualizamos el hash del archivo
				registroTmp << *it_archivoNombre << " " << 
					Hash::funcionDeHash(*it_archivoNombre) << std::endl;

				// Insertamos archivo en cola de modificados
				modificados->push(*it_archivoNombre);

				huboCambio = true;
			}
			// Caso en que no ha sido modificado
			else {
				registroTmp << reg_archivoNombre << " " << reg_archivoHash 
					<< std::endl;
			}

			// Tomamos el registro siguiente
			registro >> reg_archivoNombre >> reg_archivoHash;
		}
		// Caso en el que se han agregado nuevos archivos
		else if(*it_archivoNombre < reg_archivoNombre || registro.eof()) {
			// Registramos archivo nuevo
			registroTmp << *it_archivoNombre << " " << 
				Hash::funcionDeHash(*it_archivoNombre) << std::endl;

			// Insertamos archivo en cola de nuevos
			nuevos->push(*it_archivoNombre);

			huboCambio = true;
		}
	}

	// Encolamos los últimos registros pertenecientes a archivos eliminados
	while(!registro.eof()) {
		eliminados->push(reg_archivoNombre);
		registro >> reg_archivoNombre >> reg_archivoHash;

		huboCambio = true;
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

// Elimina un archivo o un bloque de un archivo del directorio local
// Devuelve 0 en caso de eliminar correctamente el archivo
// Por ahora solamente borra archivos enteros, num_bloque = WHOLE_FILE
int ManejadorDeArchivos::eliminarArchivo(const std::string &nombre_archivo, 
	const std::string &num_bloque) {

	std::fstream archivo;
	int cod_error = 0;

	// Busca el archivo y si lo encuentra, lo borra
	archivo.open(nombre_archivo.c_str(), std::ios_base::in);
	if (!archivo.is_open())  // No existe el archivo a eliminar
		cod_error = 1;
	else {  
		archivo.close();
		cod_error = remove(nombre_archivo.c_str());
	}

	// DEBUG
	std::cout << "Se elimino archivo con nombre: " << nombre_archivo << std::endl;
	//END DEBUG

	return cod_error;
}

// Agrega un nuevo archivo o un bloque al directorio local
// Por ahora solamente agrega archivos enteros, num_bloque = WHOLE_FILE
int ManejadorDeArchivos::agregarArchivo(const std::string &nombre_archivo, 
	const std::string &num_bloque, const std::string &bloque_archivo) {
	
	std::fstream archivo;
	int cod_error = 0;

	// Intenta abrir el archivo 
	archivo.open(nombre_archivo.c_str(), std::ios_base::in | std::ios_base::out 
		| std::ios_base::app);

	if (!archivo.is_open()) {  // El archivo no existe
		archivo.clear();

		//crea el archivo
		archivo.open(nombre_archivo.c_str(), std::fstream::out);
		archivo.close();

		// Vuelve a abrir el archivo
		archivo.open(nombre_archivo.c_str(), std::ios_base::in | std::ios_base::out 
			| std::ios_base::app);
		
		if (!archivo.is_open()) // No se pudo crear el archivo
			cod_error = 1;
	}

	if (cod_error == 0) {
		// Se escribe el contenido en el archivo
		archivo.write(bloque_archivo.c_str(), bloque_archivo.length());
	
		// Se cierra el archivo
		archivo.close();	
	}

	// DEBUG
	std::cout << "Se agrego archivo con nombre: " << nombre_archivo << std::endl;	
	//END DEBUG

	return cod_error;
}
