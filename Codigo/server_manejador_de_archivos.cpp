
#include "server_manejador_de_archivos.h"
#include <stdlib.h>
#include "dirent.h"



/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
ManejadorDeArchivos::ManejadorDeArchivos(const std::string& directorio) : 
	directorio(directorio) { }


// Destructor
ManejadorDeArchivos::~ManejadorDeArchivos() { }

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
// Devuelve el hash del archivo
int ManejadorDeArchivos::agregarArchivo(const std::string &nombre_archivo, 
	const std::string &num_bloque, const std::string &bloque_archivo, std::string &hash) {
	
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

	// Se guarda el hash del archivo
	hash = obtenerHashArchivo(bloque_archivo);

	// DEBUG
	std::cout << "Se agrego archivo con nombre: " << nombre_archivo << std::endl;	
	//END DEBUG

	return cod_error;
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
			Archivo archivo(entrada->d_name);
			//DEBUG: Hacer overload de metodo obtenerHashArchivo con char*
			std::string nombre_archivo = entrada->d_name;
			std::string hash_archivo = obtenerHashArchivo(nombre_archivo);
			archivo.asignarHash(hash_archivo);
			// DEBUG: Hardcode!
			archivo.asignarFechaDeModificacion("1");
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
