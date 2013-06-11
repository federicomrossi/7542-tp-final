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

// Devuelve un archivo que se localiza en el directorio
// De no existir, devuelve un codigo de error = 0
bool ManejadorDeArchivos::obtenerArchivo(const std::string &nombre_archivo, Archivo& archivo) {
	bool sinError = true;
	std::string hash_archivo = obtenerHashArchivo(nombre_archivo);
	if (hash_archivo.empty())
		sinError = false;
	else {
		archivo.asignarHash(hash_archivo);
		archivo.asignarFechaDeModificacion("1");
	}
	return sinError;
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
				obtenerHashArchivo(*it_archivoNombre) << std::endl;

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
			if(reg_archivoHash != obtenerHashArchivo(*it_archivoNombre)) {
				// Actualizamos el hash del archivo
				registroTmp << *it_archivoNombre << " " << 
					obtenerHashArchivo(*it_archivoNombre) << std::endl;

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
				obtenerHashArchivo(*it_archivoNombre) << std::endl;

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


// Actualiza el registro local de archivos.
// POST: se devuelve 'false' si se produjeron cambios en el registro o
// 'true' en su defecto; esto evita tener que revisar las colas para
// comprobar cambios.
bool ManejadorDeArchivos::actualizarRegistroDeArchivos() {
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
				obtenerHashArchivo(*it_archivoNombre) << std::endl;

			huboCambio = true;
			continue;
		}

		// Caso en que se han eliminado archivos
		while(*it_archivoNombre > reg_archivoNombre && !registro.eof()) {
			// Tomamos el registro siguiente
			registro >> reg_archivoNombre >> reg_archivoHash;

			huboCambio = true;
		}

		// Caso en el que el archivo se mantiene existente
		if(*it_archivoNombre == reg_archivoNombre) {
			// Corroboramos si ha sido modificado
			if(reg_archivoHash != obtenerHashArchivo(*it_archivoNombre)) {
				// Actualizamos el hash del archivo
				registroTmp << *it_archivoNombre << " " << 
					obtenerHashArchivo(*it_archivoNombre) << std::endl;

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
				obtenerHashArchivo(*it_archivoNombre) << std::endl;

			huboCambio = true;
		}
	}

	// Encolamos los últimos registros pertenecientes a archivos eliminados
	while(!registro.eof()) {
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


// Recibe una lista de archivos, compara con la que se encuentra localmente 
// * ListaExterna: lista de archivos con la cual se compara
// * Faltantes: lista de archivos que no estan en el dir local
// * Sobrantes: lista de archivos que no estan en la lista
void ManejadorDeArchivos::obtenerListaDeActualizacion(Lista<Archivo>* listaExterna, 
	Lista<Archivo>* faltantes, Lista<Archivo>* sobrantes) {

	if (!listaExterna->estaVacia()) {

		// Bloqueamos el mutex
		Lock l(m);

		// Variables auxiliares
		Lista<Archivo> listaLocal;

		// Se obtiene la lista de archivos
		obtenerArchivosDeDirectorio(&listaLocal);

		// Iterador para la lista a comparar y su tamanio
		int it_s = 0; 
		int tamLista_s = listaExterna->tamanio();
		// Iterador para la lista local del cliente y su tamanio
		int it_c = 0;
		int tamLista_c = listaLocal.tamanio();

		// Se buscan diferencias y similitudes entre ambas listas
		while ((it_s < tamLista_s) && (it_c < tamLista_c)) {  // Mientras no haya terminado alguna lista
			
			// Se toma un elemento de cada lista
			Archivo externo((*listaExterna)[it_s]);
			Archivo local(listaLocal[it_c]);

			// Si el nombre archivo de la lista es > al nombre del local, se eliminan hasta ese nombre 
				// los archivos del lado del cliente
			if (externo > local) {
				std::cout << "Comparacion: "<< externo.obtenerNombre() << " > " << local.obtenerNombre() << std::endl;
				this->eliminarArchivo(local.obtenerNombre(), WHOLE_FILE);
				it_c++;
			}

			// Si el nombre archivo de la lista es == al nombre del local, compara hash y despues fecha
			else if (externo == local) {
				std::cout << "Comparacion: "<< externo.obtenerNombre() << " == " << local.obtenerNombre() << std::endl;
				// Si el hash es igual, no se hace nada
				// Si el hash es diferente, se comparan fechas a ver cual es el mas actual
				if (externo.obtenerHash() != local.obtenerHash()) {
/*					// Si fecha_archivo de la lista <(menos actual) a la del archivo, 
					// se deberia enviar archivo
					if (externo.obtenerFechaDeModificacion() < local.obtenerFechaDeModificacion())
						sobrantes->insertarUltimo(local);
				
					// Sino, se deberia sobre escribir en cliente (se pide el archivo)
					else
						faltantes->insertarUltimo(externo);
*/				}
				// Avanzo en ambas listas
				it_s++;
				it_c++;
			}
			// Si nombre archivo de la lista es < al del archivo, se deben pedir archivos hasta ese nombre
			else {
				std::cout <<"Comparacion: "<< externo.obtenerNombre() << " < " << local.obtenerNombre() << std::endl;
				while (externo < local && it_s < tamLista_s) {
					externo = (*listaExterna)[it_s];
					faltantes->insertarUltimo(externo);
					it_s++;
				}
			}
		}  // Fin de comparacion
		// Si quedan elementos en la lista con la cual se compara
		if (it_s < tamLista_s) {  // Quedan elementos por agregar al dir local
			for (int i = it_s; i < tamLista_s; i++) {
				Archivo externo((*listaExterna)[i]);
				faltantes->insertarUltimo(externo);
				it_s++;
			}
		}
		// Si quedan elementos en la lista del dir local
		if (it_c < tamLista_c) {  // Quedan elementos 
			for (int i = it_c; i < tamLista_c; i++) {
				Archivo local(listaLocal[i]);
				this->eliminarArchivo(local.obtenerNombre(), WHOLE_FILE);
				it_c++;
			}
		}
	}
	//DEBUG
	std::cout << "Termino comparacion en actualizacion.. " << std::endl;

}


// Elimina un archivo o un bloque de un archivo del directorio local
// Devuelve 0 en caso de eliminar correctamente el archivo
// Por ahora solamente borra archivos enteros, num_bloque = WHOLE_FILE
int ManejadorDeArchivos::eliminarArchivo(const std::string &nombre_archivo, 
	const std::string &num_bloque) {

	std::fstream archivo;
	int cod_error = 0;

	std::string ruta = this->directorio + "/" + nombre_archivo;

	// Busca el archivo y si lo encuentra, lo borra
	archivo.open(ruta.c_str(), std::ios_base::in);
	if (!archivo.is_open())  // No existe el archivo a eliminar
		cod_error = 1;
	else {  
		archivo.close();
		cod_error = remove(ruta.c_str());
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

	std::string ruta = this->directorio + "/" + nombre_archivo;

	// Intenta abrir el archivo 
	archivo.open(ruta.c_str(), std::ios_base::in | std::ios_base::out 
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

//	return cod_error;
	return 0;
}

// Devuelve el hash del archivo con nombre especificado por parametros
// De no existir, se devuelve un string vacio
std::string ManejadorDeArchivos::obtenerHashArchivo(const std::string &nombre_archivo) {
	
	std::ifstream archivo;

	// DEBUG
	std::string nombre = this->directorio + "/" + nombre_archivo;

	std::string hash_archivo;

	// Intenta abrir el archivo 
	archivo.open(nombre.c_str(), std::ios_base::in);

	if (archivo.is_open()) {  // El archivo existe
		// Se obtiene la longitud del archivo
		archivo.seekg(0, archivo.end);
		int longArchivo = archivo.tellg();
		archivo.seekg(0, archivo.beg);

		// Se lee el archivo completo
		char* buffer = new char[longArchivo];
		archivo.getline(buffer, longArchivo);

		// Se calcula el hash del archvio
		hash_archivo = Hash::funcionDeHash(buffer, longArchivo);
	
		// Se cierra el archivo
		archivo.close();	

		// Se borra el buffer de entrada
		delete(buffer);
	}

	return hash_archivo;
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

// Devuelve una lista con los archivos (ordenados por nombre) que se encuentran 
// ubicados en el directorio administrado por el manejador.
void ManejadorDeArchivos::obtenerArchivosDeDirectorio(Lista<Archivo>* listaArchivos) {
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

			// Insertamos el archivo en la lista
			std::string nombre_archivo = entrada->d_name;
			Archivo archivo(nombre_archivo);
			std::string hash_archivo = obtenerHashArchivo(nombre_archivo);
			archivo.asignarHash(obtenerHashArchivo(nombre_archivo));
			// DEBUG: Hardcode!
			archivo.asignarFechaDeModificacion("1");
			if (!hash_archivo.empty())  // Se obtuvo un string vacio si no existia el archivo
				listaArchivos->insertarUltimo(archivo);
		}

		closedir(dir);
	} 
	else 
		throw "ERROR: No se ha podido abrir el directorio.";

	//DEBUG
	std::cout << "Se ordenan los archivos " << std::endl;
	listaArchivos->ordenar();

}
