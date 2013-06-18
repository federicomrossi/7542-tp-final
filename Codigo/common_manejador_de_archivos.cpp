//
//  common_manejador_de_archivos.h
//  CLASE MANEJADORDEARCHIVOS
//  


#include "common_manejador_de_archivos.h"
#include "common_parser.h"
#include "common_lista.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "dirent.h"




namespace {
	// Constantes para los nombres de directorio
	const std::string DIR_AU = ".au";

	// Constantes para los nombres de archivo
	const std::string ARCHIVO_REG_ARCHIVOS = ".reg_archivos";

	// Delimitador de campos del registro
	const std::string DELIMITADOR = ",";

	// Tamanio buffer lectura
	#define TAM_BUF 2048
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
	unsigned char esDirectorio = 0x4;

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

// Devuelve una lista con los archivos (ordenados por nombre) que se 
// encuentran ubicados en el registro administrado por el manejador.

void ManejadorDeArchivos::obtenerArchivosDeRegistro(Lista< std::pair 
	<std::string, std::string > >* listaArchivos) {
	
	Lock l(m);

	// variables auxiliares
	std::string nombre, hash, linea;
	

	// Armamos ruta del registro
	std::string registro = this->directorio + DIR_AU + "/" 
		+ ARCHIVO_REG_ARCHIVOS;
	
	// se abre el archivo
	std::ifstream archivo(registro.c_str(), std::ios_base::in);

	if (archivo.is_open()) {
		// Se leen y guardan los nombres de archivos + hash en la lista
		while(std::getline(archivo, linea)) {
			separarNombreYHash(linea, nombre, hash);
			std::pair<std::string, std::string> par(nombre, hash);
			listaArchivos->insertarUltimo(par);
		}

		// Se cierra el archivo
		archivo.close();
	}

	listaArchivos->ordenar();
}


// Agrega un nuevo archivo al directorio.
// PRE: 'nombreArchivo' es el nombre del archivo nuevo; 'contenido' es el
// contenido del archivo nuevo expresado en formato hexadecimal como una
// cadena.
// POST: devuelve true si se agregó el archivo con éxito o false en caso
// contrario
void ManejadorDeArchivos::agregarArchivo(const std::string& nombreArchivo, 
	const std::string& contenido) {
	// Variables auxiliares
	std::fstream archivo;

	// Armamos ruta del archivo
	std::string ruta = this->directorio + "/" + nombreArchivo;

	// Intenta abrir el archivo 
	archivo.open(ruta.c_str(), std::ios_base::out | std::ios_base::app);

	if (!archivo.is_open()) {  // El archivo no existe
		archivo.clear();

		//crea el archivo
		archivo.open(nombreArchivo.c_str(), std::fstream::out);
		archivo.close();

		// Vuelve a abrir el archivo
		archivo.open(nombreArchivo.c_str(), std::ios_base::out | std::ios_base::app);
		
		if (!archivo.is_open()) // No se pudo crear el archivo
			throw "ERROR: Archivo nuevo no pudo ser creado.";
	}

	// Se convierte el archivo de hexa a char nuevamente
	std::string archivoBin = (char*)Convertir::htoui(contenido);

	// Se escribe el contenido en el archivo
	archivo.write(archivoBin.c_str(), archivoBin.length());

	// Se cierra el archivo
	archivo.close();

	// DEBUG
	std::cout << "Se agrego archivo con nombre: " << nombreArchivo << std::endl;	
	//END DEBUG

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


// Guarda modificaciones de bloques en un archivo, dejandolo de menor o mayor
// tamanio segun corresponda
void ManejadorDeArchivos::modificarArchivo(std::string& nombreArchivo, 
	int cantBloquesDelArchivo, Lista< std::pair< int, std::string > >& 
	listaBloquesAReemplazar) {

/*	// Var auxiliares
	std::fstream archivo;
	std::fstream temp;
	std::string aux, nombreArchivoTemp = "KDSF988G98G9S8ASDF8D8F78E78D8F787D8F7Q~";
	
	// Abre un archivo 
	archivo.open(nombreArchivo.c_str(), std::ios_base::in);	

	// Si no existe, error
	if (!archivo.is_open())
		throw "ERROR: Archivo no existe.";


	// Sino, se cfrea el otro
	temp.open(nombreArchivoTemp.c_str(), std::ios_base::out);
	if (!temp.is_open())
		throw "ERROR: No se pudo crear arcihvo.";

	// se cierra y vuelve a abrir para escribir
	temp.close();
	temp.open(nombreArchivoTemp.c_str(), std::ios_base::out | 
		std::ios_base::app);

	// Se va al principio del archivo
	archivo.seekg(0, std::ios_base::beg);

	// Se copian los bloques que no fueron modificados
	int numBloque = listaBloquesAReemplazar.verPrimero().first;
	for (int i = 0; i < numBloque; i++) {
		aux = obtenerContenido(nombreArchivo);
		archivo.write((char*)Convertir::htoui(aux), aux.length()/2);
	}

*/	// Se compara
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
	// Bloqueamos el mutex
	Lock l(m);

	// Armamos la ruta hacia el archivo
	std::string ruta = this->directorio + "/" + nombreArchivo;

	// Abrimos el archivo
	std::ifstream archivo(ruta.c_str(), 
		std::ios::in | std::ios::binary | std::ios::ate);

	if(!archivo.is_open())
		throw "ERROR: Archivo de entrada inválido.";

	// Variables auxiliares
	int size;
	uint8_t * contenidoTemp;
	int inicio, fin;

	// Almacenamos momentaneamente el contenido del archivo original
	size = archivo.tellg();
	if (numBloque == 0) {
		inicio = 0;
		fin = size;
	}
	else {
		inicio = TAMANIO_BLOQUE * (numBloque - 1);
		fin = inicio + TAMANIO_BLOQUE;
	}
	if (inicio < fin) {
		// Se posiciona en el bloque correspondiente
		archivo.seekg(inicio);

		// Se crea e inicializa una variable contenedora
		contenidoTemp = new uint8_t[fin - inicio];
		memset(contenidoTemp, '\0', fin - inicio);

		// se lee del archivo
		archivo.read((char*)contenidoTemp, fin - inicio);

		// Convertimos el contenido a hexadecimal
		std::string contenidoHex(Convertir::uitoh((uint8_t*)contenidoTemp, (size_t)(fin - inicio)));

		// se devuelve el contenido
		delete[] contenidoTemp;
		return contenidoHex;
	}

	archivo.close();

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


// Devuelve la cantidad de bloques de un archivo
// Si no existe el archivo, devuelve -1
int ManejadorDeArchivos::obtenerCantBloques(const std::string &nombreArchivo) {
	// Armamos la ruta hacia el archivo
	std::string ruta = this->directorio + "/" + nombreArchivo;

	// Variables auxiliares	
	int longitud, cantBloques = -1;

	// Se busca el archivo
	std::fstream archivo;

	// Se abre el archivo
	archivo.open(ruta.c_str(), std::ios_base::in | std::ios_base::binary 
		| std::ios_base::ate);

	// Si no existe, se devuelve -1
	if (archivo.is_open()) {
		// Se obtiene longitud archivo
		longitud = archivo.tellg();

		// Se calcula cantBloques
		cantBloques = floor((double)(longitud/TAMANIO_BLOQUE));
		
		if (longitud % TAMANIO_BLOQUE > 0)
			cantBloques++;
	}
	
	// Se devuelve la cantidad de bloques
	return(cantBloques);
}


// Recibe una lista de archivos, compara con la que se encuentra localmente 
// * ListaExterna: lista de archivos con la cual se compara
// * Faltantes: lista de archivos que no estan en el dir local
// * Sobrantes: lista de archivos que no estan en la lista que se deben 
// eliminar del dir local
void ManejadorDeArchivos::obtenerListaDeActualizacion(
	Lista< std::pair< std::string, std::pair< std::string, 
	int > > >* listaExterna, Lista< std::pair< std::string, 
	Lista<int> > >* faltantes, Lista<std::string>* sobrantes) {
	// La primer lista contiene nombre, hash y cantidad de bloques (en ese 
	// orden). La segunda tiene hash y una cola de numeros de bloque.

	// Variables auxiliares
	std::pair< std::string, std::pair< std::string, int > > externo;
	std::pair< std::string, std::string> registro;

	// Se crea una lista y se guarda una lista de archivos en registro
	Lista< std::pair< std::string, std::string> > listaRegistro;
	obtenerArchivosDeRegistro(&listaRegistro);

	// Iterador para la lista a comparar y su tamanio
	int it_e = 0; 
	int tam_e = listaExterna->tamanio();
	// Iterador para la lista local del cliente y su tamanio
	int it_r = 0;
	int tam_r = listaRegistro.tamanio();

	// Se buscan diferencias y similitudes entre ambas listas
	while ((it_e < tam_e) && (it_r < tam_r)) {  
		// Mientras no haya terminado alguna lista
		// Se obtiene un elemento de cada lista
		externo = (*listaExterna)[it_e];
		registro = listaRegistro[it_r];

		// Caso en que el nombre archivo de la listaExterna es > al nombre del
		// registro
		if (externo.first > registro.first) {
			// DEBUG
			std::cout << "Comparacion: "<< externo.first << " > " << registro.first << std::endl;
			// Se agrega a lista eliminar
			sobrantes->insertarUltimo(registro.first);
			it_r++;
		}
		// Caso de nombre archivo de la listaExterna es < al del registro 
		else if (externo.first < registro.first) {
			//DEBUG
			std::cout <<"Comparacion: "<< externo.first << " < " << registro.first << std::endl;
			// Se deben pedir archivos hasta 
			while (externo.first < registro.first && it_e < tam_e) {
				externo = (*listaExterna)[it_e];
				Lista<int> num_bloques;
				num_bloques.insertarUltimo(0);
				std::pair< std::string, Lista<int> > nom_num_bloques(
					externo.first, num_bloques);
				faltantes->insertarUltimo(nom_num_bloques);
				it_e++;
			}
		}
		else {  // Caso de nombres iguales
			//DEBUG
			std::cout <<"Comparacion: "<< externo.first << " == " << registro.first << std::endl;
			// Compara hashes
			if (externo.second.first != registro.second) {
				//DEBUG
				std::cout <<"Hashes distintos" << std::endl;
				Lista<int> num_bloques;
				obtenerListaDiferencias(externo.first, externo.second.second,
					&num_bloques);
				std::pair< std::string, Lista<int> > nom_num_bloques = 
					std::make_pair(externo.first, num_bloques);
				faltantes->insertarUltimo(nom_num_bloques);
			}
			// Avanzo en ambas listas
			it_e++;
			it_r++;
			// Si alguna termina, retrocedo la otra
			if (it_e >= tam_e && it_r < tam_r)
				it_r--;
			if (it_r >= tam_r && it_e < tam_e)
				it_e--;
		}
	}  // end while
	// Si quedan elementos en la listaExterna
	if (it_e < tam_e) {  // Quedan elementos por agregar al dir local
		for (int i = it_e; i < tam_e; i++) {
			externo = (*listaExterna)[it_e];
			Lista<int> num_bloques;
			num_bloques.insertarUltimo(0);
			std::pair< std::string, Lista<int> > nom_num_bloques(externo.first, 
				num_bloques);
			faltantes->insertarUltimo(nom_num_bloques);
			it_e++;
		}
	}
	// Si quedan elementos en la lista del dir local
	if (it_r < tam_r) {  // Quedan elementos por eliminar en el dir local
		for (int i = it_r; i < tam_r; i++) {
			externo = (*listaExterna)[it_e];
			sobrantes->insertarUltimo(externo.first);
			it_r++;
		}
	}
}


// Devuelve las diferencias que existen entre 2 archivos
void ManejadorDeArchivos::obtenerListaDiferencias(std::string nombre, 
	int cantBloques, Lista<int>* diferencias) {
	diferencias->insertarUltimo(0);
}


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
	Lista< std::string > ld;
	this->obtenerArchivosDeDirectorio(&ld);

	// Variables auxiliares de procesamiento
	std::string reg_archivoNombre, reg_archivoHash;
	char buffer[TAM_BUF];
	bool eof = false;

	// Tomamos el primer registro
	if(!registro.getline(buffer, TAM_BUF)) eof = true;
	this->separarNombreYHash(buffer, reg_archivoNombre, reg_archivoHash);

	// Iteramos sobre los nombres de archivos existentes en el directorio
	for(size_t i = 0; i < ld.tamanio(); i++) {
		// Caso en el que no hay mas registros y se han agregado archivos
		if(eof) {
			// Calculamos el hash del archivo nuevo
			std::string hashNuevo;
			this->obtenerHash(ld[i], hashNuevo);

			// Registramos archivo nuevo
			registroTmp << ld[i] << DELIMITADOR << hashNuevo << std::endl;

			// Insertamos archivo en cola de nuevos
			nuevos->push(ld[i]);

			huboCambio = true;
			continue;
		}

		// Caso en que se han eliminado archivos
		while(ld[i] > reg_archivoNombre && !eof) {
			// Insertamos en cola de eliminados
			eliminados->push(reg_archivoNombre);

			// Tomamos el registro siguiente
			memset(buffer, 0, TAM_BUF);
			if(!registro.getline(buffer, TAM_BUF)) eof = true;
			this->separarNombreYHash(buffer, reg_archivoNombre, 
				reg_archivoHash);

			huboCambio = true;
		}

		// Caso en el que el archivo se mantiene existente
		if(ld[i] == reg_archivoNombre) {
			// Corroboramos si ha sido modificado
			// [FALTA MODIFICAR ESTA PARTEEEEE]
			if(false) {
			// if(reg_archivoHash != obtenerHash(ld[i])) {
			// 	// Actualizamos el hash del archivo
			// 	registroTmp << ld[i] << " " << 
			// 		obtenerHash(ld[i]) << std::endl;

			// 	// Insertamos archivo en cola de modificados
			// 	modificados->push(ld[i]);

			// 	huboCambio = true;
			}
			// [FIN FALTA MODIFICAR ESTA PARTEEEEE]
			// Caso en que no ha sido modificado
			else {
				registroTmp << reg_archivoNombre << DELIMITADOR 
					<< reg_archivoHash << std::endl;
			}

			// Tomamos el registro siguiente
			memset(buffer, 0, TAM_BUF);
			if(!registro.getline(buffer, TAM_BUF)) eof = true;
			this->separarNombreYHash(buffer, reg_archivoNombre, 
				reg_archivoHash);
		}
		// Caso en el que se han agregado nuevos archivos
		else if(ld[i] < reg_archivoNombre || eof) {
			// Calculamos el hash del archivo nuevo
			std::string hashNuevo;
			this->obtenerHash(ld[i], hashNuevo);
			// Registramos archivo nuevo
			registroTmp << ld[i] << DELIMITADOR << hashNuevo << std::endl;

			// Insertamos archivo en cola de nuevos
			nuevos->push(ld[i]);

			huboCambio = true;
		}
	}

	// Encolamos los últimos registros pertenecientes a archivos eliminados
	while(!eof) {
		eliminados->push(reg_archivoNombre);

		// Tomamos el registro siguiente
		memset(buffer, 0, TAM_BUF);
		if(!registro.getline(buffer, TAM_BUF)) eof = true;
		this->separarNombreYHash(buffer, reg_archivoNombre,	reg_archivoHash);

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





/*
 * IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */


// Separa de una linea el nombre y el hash
void ManejadorDeArchivos::separarNombreYHash(const std::string &linea, std::string& nombre,
	std::string &hash) {

	// Se limpian las variables
	nombre.clear();
	hash.clear();
	
	// Se separa si hay contenido
	if (!linea.empty()) {
		// Se busca el ultimo espacio
		int delim = linea.find_last_of(DELIMITADOR[0]);

		// Se guarda nombre y hash por separado
		nombre = linea.substr(0, delim);

		hash = linea.substr(delim + 1);
	}
}



// Devuelve un string de caracteres alfanumericos aleatorios de 
// tamanio 'longitud'
void ManejadorDeArchivos::randomString(std::string &s, int longitud) {
	// Tabla de caracteres posibles
	char alphanumerico[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	// Se randomiza el set
	srand(time(NULL));
	int tamanio = sizeof(alphanumerico) - 1;

	for (int i = 0; i < longitud; ++i) 
		s[i] = alphanumerico[rand() % tamanio];
}
