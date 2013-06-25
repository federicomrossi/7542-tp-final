#include "server_recolector_de_informacion.h"

// Devuelve la cantidad de bytes almacenados en el servidor
// recorriendo todos los directorios que se encuentran a partir
// del raiz. Soporta directorios anidados
int Recolector::cantidadBytesAlmacenados() {
	// Variables auxiliares
	std::string dirActual, dirAux, nombreEntrada;
	int tamTotal = 0;
	std::stack< std::string > pilaDirectorios;
	// Variables para recorrer directorios
	DIR *dir;
	struct dirent *entrada = 0;
	struct stat file;
	unsigned char esDirectorio = 0x4;

	// Se obtiene el directorio raiz
	Configuracion conf;
	std::string directorio = conf.obtenerPath();

	// Inserto el directorio raiz en la pila
	pilaDirectorios.push(directorio);

	// Mientras pila no vacia
	while (!pilaDirectorios.empty()) {
		// Levanto la direccion actual
		dirActual = pilaDirectorios.top();
		pilaDirectorios.pop();

		// Abrimos directorio
		dir = opendir(dirActual.c_str());

		// Iteramos sobre cada objeto del directorio
		while ((entrada = readdir (dir)) != NULL) {
			// Guardo el nombre de la entrada
			nombreEntrada = entrada->d_name;

			// Si es directorio, se guarda en pila
			if (entrada->d_type == esDirectorio) {
				if (nombreEntrada != "." && nombreEntrada != "..") {
					// Se guarda el path entero
					dirAux = dirActual + nombreEntrada + "/";

					// Se agrega el path a la pila de directorios
					pilaDirectorios.push(dirAux);
				}
			}
			// Sino, se guarda cantidad de bytes
			else {
				// Se obtiene el path entero
				dirAux = dirActual + nombreEntrada;

				// Se obtiene la info del archivo
				stat(dirAux.c_str(), &file);

				// Se suma el tamanio al total
				tamTotal += file.st_size;
			}

		}
		closedir(dir);
	}
	return tamTotal;
}
