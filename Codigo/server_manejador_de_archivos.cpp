
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
	hash = Hash::funcionDeHash(bloque_archivo);

	// DEBUG
	std::cout << "Se agrego archivo con nombre: " << nombre_archivo << std::endl;	
	//END DEBUG

	return cod_error;
}

// Devuelve la lista de archivos que se encuentran en el servidor
std::string ManejadorDeArchivos::obtenerListaDeArchivos() {
	// Variables auxiliares
	DIR *dir;
	struct dirent *entrada = 0;
	std::string listaArchivos;
	unsigned char esDirectorio =0x4;

	// Abrimos directorio y procesamos si fue exitosa la apertura
	if((dir = opendir (this->directorio.c_str())) != NULL) {
		// Iteramos sobre cada objeto del directorio
		while ((entrada = readdir (dir)) != NULL) {
			// Salteamos directorios
			if (entrada->d_type == esDirectorio)
				continue;

			// Insertamos el nombre de archivo en la lista y su hash
			listaArchivos.append(entrada->d_name);
			listaArchivos.append(" ");
			listaArchivos.append(Hash::funcionDeHash(entrada->d_name, strlen(entrada->d_name)));
			listaArchivos.append("\n");
		}

		closedir(dir);
	} 
	else 
		throw "ERROR: No se ha podido abrir el directorio.";
	// DEBUG
	std::cout << "Lista de arcihvos en sever: " <<listaArchivos << std::endl;
	//END DEBUG

	return listaArchivos;
}

