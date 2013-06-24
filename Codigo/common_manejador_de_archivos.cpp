//
//  common_manejador_de_archivos.h
//  CLASE MANEJADORDEARCHIVOS
//  


#include "common_manejador_de_archivos.h"
#include "common_parser.h"
#include "common_lista.h"
#include "common_hash.h"
#include "common_utilidades.h"
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

	// Constante que define el tamaño de los bloques de archivos en cantidad
	// de caracteres hexadecimales (ej: si se quiere un tamaño de bloque de
	// 10 Bytes, se debe insertar el valor 20).
	// const int TAMANIO_BLOQUE = 20;			// 10 bytes por bloque
	// const int TAMANIO_BLOQUE = 524288;		// 256K por bloque
	const int TAMANIO_BLOQUE = 2097152;			// 1Mb por bloque

	// Constante que define el tamaño de los bloques de hash de archivos.
	const int TAMANIO_BLOQUE_HASH = 40;

	// Longitud de nombre del archivo temporal
	const int LONGITUD_TEMP = 40;
}





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
ManejadorDeArchivos::ManejadorDeArchivos(const std::string& directorio, 
	Logger *logger) : directorio(directorio), logger(logger) { }


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
	else {
		// Mensaje de log
		this->logger->emitirLog("ERROR: No se ha podido abrir el directorio " +this->directorio);
		throw "ERROR: No se ha podido abrir el directorio.";
	}

	// Ordenamos la lista de archivos
	listaArchivos->ordenar();
}


// Devuelve una lista con los archivos (ordenados por nombre) que se 
// encuentran ubicados en el registro administrado por el manejador.
void ManejadorDeArchivos::obtenerArchivosDeRegistro(Lista<std::string>* listaArchivos,
	Lista< std::pair< std::string, std::string> >* listaPares) {
	// Bloqueamos el mutex
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
			std::pair< std::string, std::string> datos = 
				make_pair(nombre, hash);
			listaPares->insertarUltimo(datos);
			listaArchivos->insertarUltimo(nombre);
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
	// Bloqueamos el mutex
	Lock l(m);

	// Variables auxiliares
	std::fstream archivo;

	// Armamos ruta del archivo
	std::string ruta = this->directorio + "/" + nombreArchivo;

	// Intenta abrir el archivo 
	archivo.open(ruta.c_str(), std::ios_base::in);
	
	// Si abre, se elimina
	if (archivo.is_open()) {
		// Se cierra
		archivo.close();

		// Se elimina
		remove(ruta.c_str());
	}
		
	// Se crea
	archivo.open(ruta.c_str(), std::ios_base::out | std::ios_base::app);
	
	// No se pudo crear el archivo
	if (!archivo.is_open()) {
		// Mensaje de log
		this->logger->emitirLog("ERROR: Archivo nuevo " + nombreArchivo + 
			" no pudo ser creado.");
		throw "ERROR: Archivo nuevo no pudo ser creado.";
	}

	// Se convierte el archivo de hexa a char nuevamente
	uint8_t *archivoBin = Convertir::htoui(contenido);
	size_t len = contenido.size() / 2;

	// Se escribe el contenido en el archivo
	archivo.write((char*) archivoBin, len);

	// Se cierra el archivo
	archivo.close();

}


// Elimina un archivo del directorio.
// PRE: 'nombreArchivo' es el nombre de archivo.
// POST: devuelve true si se eliminó con éxito o false en su defecto.
bool ManejadorDeArchivos::eliminarArchivo(const std::string& nombreArchivo) {
	// Bloqueamos el mutex
	Lock l(m);

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


// Realiza modificaciones sobre los bloques de un archivo.
// PRE: 'nombreArchivo' es el nombre del archivo a modificar;
// 'cantloquesDelArchivo' es la cantidad nueva de bloques que debe
// contener el archivo; 'listaBloquesAReemplazar' es una lista que
// contiene los números de bloque y su respectivo contenido, los
// cuales reemplazarán a los bloques actuales.
void ManejadorDeArchivos::modificarArchivo(std::string& nombreArchivo, 
	unsigned int cantBytesDelArchivo, Lista< std::pair< int, std::string > >& 
	listaBloquesAReemplazar) {
	// Variables auxiliares
	std::fstream archivo, archivoTemp;
	std::string sRandom;
	Utilidades::randomString(LONGITUD_TEMP, sRandom);
	std::string nombreArchivoTemp = "." + sRandom + "~";
	unsigned int cantBytesParcial = 0;
	unsigned int cantBytesPorBloque = TAMANIO_BLOQUE / 2;
	int i = 1;

	// Armamos ruta de archivos
	std::string ruta = this->directorio + "/" + nombreArchivo;
	std::string rutaTemp = this->directorio + "/" + nombreArchivoTemp;

	// Intentamos abrir el archivos
	archivo.open(ruta.c_str(), std::ios::in);
	archivoTemp.open(rutaTemp.c_str(), std::ios_base::out | 
		std::ios_base::app);

	// Verificamos si la apertura fue exitosa
	if(!archivo.is_open() || !archivoTemp.is_open()) {
		// Mensaje de log
		this->logger->emitirLog("ERROR: El archivo " + nombreArchivo + 
				" no pudo ser abierto.");	
		throw "ERROR: El archivo no pudo ser abierto.";
	}


	// Vamos insertando bytes, reemplazando los actualizados hasta
	// llegar al tamaño en bytes que debe tener el archivo
	while(cantBytesParcial < cantBytesDelArchivo) {
		// Variable auxiliar para el contenido
		std::string contenidoBloque;

		// Verificamos si debe reemplazarse bloque
		if(!listaBloquesAReemplazar.estaVacia() && 
			(i == listaBloquesAReemplazar.verPrimero().first)) {
			// Insertamos el contenido nuevo en el bloque
			contenidoBloque = listaBloquesAReemplazar.verPrimero().second;
			listaBloquesAReemplazar.eliminarPrimero();
		}
		else
			contenidoBloque = this->obtenerContenido(nombreArchivo, i);
		
		// Corroboramos si debemos truncar el bloque
		unsigned int v = cantBytesDelArchivo - cantBytesParcial;

		if(v < cantBytesPorBloque)
			// Obtenemos la cantidad de bytes que necesitamos del bloque
			contenidoBloque = contenidoBloque.substr(0, v * 2);

		// Se convierte el contenido de hexa a char nuevamente
		uint8_t *contenidoBloqueBin = Convertir::htoui(contenidoBloque);
		size_t len = contenidoBloque.size() / 2;

		// Se escribe el contenido en el archivo
		archivoTemp.write((char*) contenidoBloqueBin, len);

		// Incrementamos la cantidad parcial de bytes a guardar
		cantBytesParcial += len;

		i++;
	}

	archivo.close();
	archivoTemp.close();

	// Eliminamos el archivo original y convertimos el temporal en el oficial
	remove(ruta.c_str());
	rename(rutaTemp.c_str(), ruta.c_str());
}


// Comprueba la existencia de un archivo en el directorio.
// PRE: 'nombreArchivo' es el nombre de archivo a buscar.
// POST: devuelve true si existe o false en caso contrario.
bool ManejadorDeArchivos::existeArchivo(std::string& nombreArchivo) {
	// Variables auxiliares
	Lista<std::string> l;

	// Relevamos los archivos del directorio
	this->obtenerArchivosDeDirectorio(&l);

	return l.buscar(nombreArchivo);
}


// Calcula el hash del archivo, el cual se encuentra conformado
// por los hashes de cada bloque concatenados.
// PRE: 'nombreArchivo' es el nombre de archivo, 'hashArchivo' es
// el string en donde se depositará el hash.
// POST: se devuelve la cantidad de bloques que posee actualmente el
// archivo.
int ManejadorDeArchivos::obtenerHash(const std::string& nombreArchivo, 
	std::string& hashArchivo) {
	// Limpiamos el argumento en donde se depositará el hash del contenido
	hashArchivo.clear();

	// Obtenemos la cantidad de bloques del archivo
	int cantBloques = this->obtenerCantBloques(nombreArchivo);

	for(int i = 1; i <= cantBloques; i++) {
		// Obtenemos el bloque i del contenido
		std::string bloque = this->obtenerContenido(nombreArchivo, i);

		if(i == cantBloques) {
			unsigned int bytes = this->obtenerCantBytes(nombreArchivo);
			
			// Quiltamos el relleno
			bloque = bloque.substr(0, (bytes * 2) - (TAMANIO_BLOQUE * 
				(i - 1)));
		}

		// Concatenamos el hash del bloque
		hashArchivo.append(Hash::funcionDeHash(bloque));
	}

	return cantBloques;
}


// Devuelve el hash del archivo, el cual se encuentra conformado
// por los hashes de cada bloque concatenados.
// PRE: 'nombreArchivo' es el nombre de archivo de registro, 
// 'listaNombreHashReg' es una lista de pares de <nombre,hash> del 
// archivo registro
// POST: Se devuelve el hash
std::string ManejadorDeArchivos::obtenerHashRegistro(Lista< std::pair< 
	std::string, std::string> > * listaNombreHashReg, std::string& nombreArchivo) {
	// Variables auxiliares
	int i, tam = listaNombreHashReg->tamanio();
	std::pair< std::string, std::string> par;
	std::string hash;

	// Se busca en la lista
	for (i = 0; i < tam; i++) {
		// Se levanta un elemento
		par = (*listaNombreHashReg)[i];
		// Si se encuentra, se guarda el hash
		if (par.first == nombreArchivo) {
			hash = par.second;
			break;
		}
	}
	return hash;
}


// Devuelve el hash del bloque de un archivo.
// PRE: 'nombreArchivo' es el nombre de archivo del bloque; 'numBloque'
// es el número de bloque del que se desea obtener el hash.
// POST: se devuelve una cadena con el hash del bloque.
std::string ManejadorDeArchivos::obtenerHashDeBloque(
	const std::string& nombreArchivo, int numBloque) {
	// Tomamos el contenido del bloque
	std::string contenido = this->obtenerContenido(nombreArchivo, numBloque);
	int cantTotalBloques = this->obtenerCantBloques(nombreArchivo);

	// Si el bloque es el mayor de todos, le quitamos el relleno de ceros
	if(cantTotalBloques == numBloque) {
		unsigned int bytes = this->obtenerCantBytes(nombreArchivo);
		
		// Quiltamos el relleno
		contenido = contenido.substr(0, (bytes * 2) - (TAMANIO_BLOQUE * 
			(numBloque - 1)));
	}

	// Si no existe el bloque, no devolvemos nada
	if(contenido == "") return "";

	return Hash::funcionDeHash(contenido);
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
		inicio = (TAMANIO_BLOQUE / 2) * (numBloque - 1);
		fin = inicio + (TAMANIO_BLOQUE / 2);
	}

	if(inicio < fin) {
		// Se posiciona en el bloque correspondiente
		archivo.seekg(inicio);

		// Se crea e inicializa una variable contenedora
		contenidoTemp = new uint8_t[fin - inicio];
		memset(contenidoTemp, '\0', fin - inicio);

		// se lee del archivo
		archivo.read((char*)contenidoTemp, fin - inicio);

		// Convertimos el contenido a hexadecimal
		std::string contenidoHex(Convertir::uitoh((uint8_t*)contenidoTemp, (size_t)(fin - inicio)));

		// Se devuelve el contenido
		delete[] contenidoTemp;
		return contenidoHex;
	}

	archivo.close();

	return "";
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
		// Se obtiene longitud archivo y se la multiplica por 2 para
		// considerarlo en hexa
		longitud = archivo.tellg();

		// Se calcula cantBloques
		cantBloques = floor((double)(longitud/(TAMANIO_BLOQUE/2)));
		
		if (longitud % (TAMANIO_BLOQUE/2) > 0)
			cantBloques++;
	}
	
	// Se devuelve la cantidad de bloques hexadecimales que hay
	return cantBloques;
}


// Devuelve la cantidad de Bytes de un archivo
// PRE: 'nombreArchivo' es el nombre de archivo. El archivo no debe
// sobrepasar los 4Gb de tamaño.
// POST: en caso de no poder abrir el archivo (a causa de no existencia),
// se devuelve 0. Se recomienda al usuario verificar la existencia
// previamente para no confundir el cero de error con el valor nulo de
// que puede poseer cierto archivo.
unsigned int ManejadorDeArchivos::obtenerCantBytes(
	const std::string &nombreArchivo) {
	// Bloqueamos el mutex
	Lock l(m);

	// Variables auxiliares
	unsigned int cantBytes = 0;

	// Armamos la ruta hacia el archivo
	std::string ruta = this->directorio + "/" + nombreArchivo;

	// Abrimos archivo
	std::ifstream archivo(ruta.c_str(), 
		std::ios::in | std::ios::binary | std::ios::ate);

	// Si no pudo ser abierto, lo pasamos por alto
	if(!archivo.is_open()) return 0;

	// Contabilizamos bytes
	cantBytes = archivo.tellg();

	// Cerramos archivo
	archivo.close();

	return cantBytes;
}


// Procesa dos hashes pertenecientes al contenido de un archivo y
// obtiene los bloques que han cambiado.
// PRE: 'hashViejo' y 'hashNuevo' son los hashes de los archivos a
// procesar; 'cantNuevaBloques' es la cantidad de bloques del archivo
// que es representado por 'hashNuevo'
// POST: se listan en 'listaBLoquesDiferentes' los numero de bloques
// que han cambiado; Se devuelve true si se encontraron diferencias o 
// false en caso contrario.
bool ManejadorDeArchivos::obtenerDiferencias(std::string& hashViejo,
	std::string& hashNuevo, int& cantNuevaBloques,
	Lista<int> *listaBloquesDiferentes) {
	// Si los hashes refieren a archivos vacios, devolvemos false
	if(hashViejo == "" && hashNuevo == "") return false;

	int cantViejaBloques = (hashViejo.size()) / TAMANIO_BLOQUE_HASH;

	// Caso en que el tamaño del hashViejo es nulo
	if(hashViejo.size() == 0) {
		// Insertamos todos los números de bloques
		for(int i = 1; i <= cantNuevaBloques; i++)
			listaBloquesDiferentes->insertarUltimo(i);

		return true;
	}

	// Flag para notificar cambios
	bool hubieronCambios = false;


	// Contador de bloques de hash viejo
	int i = 0;

	// Iteramos sobre los bloques
	while((i < cantViejaBloques) && (i < cantNuevaBloques)) {
		// Obtenemos el bloque i del hash viejo
		std::string bloqueViejo = hashViejo.substr(i * TAMANIO_BLOQUE_HASH,
			TAMANIO_BLOQUE_HASH);

		// Obtenemos el bloque i del hash nuevo
		std::string bloqueNuevo = hashNuevo.substr(i * TAMANIO_BLOQUE_HASH,
			TAMANIO_BLOQUE_HASH);

		// Incrementamos contador de bloques
		i++;

		// Si son iguales los bloques, sigo de largo
		if(bloqueViejo == bloqueNuevo) continue;

		// Insertamos el número de bloque en la lista
		listaBloquesDiferentes->insertarUltimo(i);
		hubieronCambios = true;
	}

	// Agregamos bloques remanentes
	while(i < cantNuevaBloques) {
		// Insertamos el número de bloque en la lista
		listaBloquesDiferentes->insertarUltimo(i + 1);
		hubieronCambios = true;

		// Incrementamos el número de bloque
		i++;
	}

	// Caso en que no hay cambios pero se ha achicado el tamaño del hash
	if(!hubieronCambios && (cantViejaBloques > cantNuevaBloques))
		hubieronCambios = true;

	return hubieronCambios;
}


// Recibe una lista de archivos, compara con la que se encuentra localmente 
// * ListaExterna: lista de archivos con la cual se compara
// * Faltantes: lista de archivos que estan modificados en el dir local
// * Sobrantes: lista de archivos que no estan en la lista que se deben 
// eliminar del dir local
// * Nuevos: lista de archivos que no estan en el dir local
void ManejadorDeArchivos::obtenerListaDeActualizacion(
	Lista< std::pair< std::string, std::pair< std::string, 
	int > > >* listaExterna, Lista< std::pair< std::string, 
	Lista<int> > >* faltantes, Lista<std::string>* sobrantes) {
	// La primer lista contiene nombre, hash y cantidad de bloques (en ese 
	// orden). La segunda tiene hash y una cola de numeros de bloque.

	// Variables auxiliares
	std::pair< std::string, std::pair< std::string, int > > externo;
	std::string hash, registro;

	// Si esta vacia, no realizar accion
	if (listaExterna->estaVacia())
		return;

	// Se crea una lista y se guarda una lista de archivos en registro
	Lista<std::string> listaRegistro;
	Lista< std::pair< std::string, std::string> > listaNombreHashReg;
	obtenerArchivosDeRegistro(&listaRegistro, &listaNombreHashReg);

	// Iterador para la lista a comparar y su tamanio
	int it = 0; 
	int tam = listaExterna->tamanio();

	for (it = 0; it < tam; it++) {
		// Se lee un item
		externo = (*listaExterna)[it];

		// Si no existe en el local o en el registro, 
		// lo guarda en faltantes
		if (!existeArchivo(externo.first) || 
			!listaRegistro.buscar(externo.first)) {

			// Se pide todo el archivo
			Lista<int> bloques;
			bloques.insertarUltimo(0);
			std::pair< std::string, Lista<int> > aPedir = 
				std::make_pair(externo.first, bloques);
			faltantes->insertarUltimo(aPedir);
		}
		// Existe el archivo en registro, entonces se comparan
		// los respectivos hashes
		else {
			// Se obtiene el hash
			hash = obtenerHashRegistro(&listaNombreHashReg,
				externo.first);

			// Si son distintos los hashes
			if (hash != externo.second.first) {
				Lista<int> bloques;

				// Se buscan las diferencias
				obtenerDiferencias(hash, externo.second.first,
					externo.second.second, &bloques);

				// Se deben pedir las diferencias
				if (!bloques.estaVacia()) {
					std::pair< std::string, Lista<int> > aPedir = 
						std::make_pair(externo.first, bloques);
					faltantes->insertarUltimo(aPedir);
				}
			}
			// Si los hashes son iguales, entonces se busca el
			// archivo y se compara realmente
			else {
				obtenerHash(externo.first, hash);
				// Si son distintos los hashes
				if (hash != externo.second.first) {
					Lista<int> bloques;

					// Se buscan las diferencias
					obtenerDiferencias(hash, externo.second.first,
						externo.second.second, &bloques);

					// Se deben pedir las diferencias
					if (!bloques.estaVacia()) {
						std::pair< std::string, Lista<int> > aPedir = 
							std::make_pair(externo.first, bloques);
						faltantes->insertarUltimo(aPedir);
					}
				}			
			}
			// Se elimina para luego saber los que sobran
			listaRegistro.eliminar(externo.first);
		}
	}
	// Se guardan en lista de sobrantes los que queden en lista registro
	tam = listaRegistro.tamanio();

	for (it = 0; it < tam; it++) {
		registro = listaRegistro[it];
		sobrantes->insertarUltimo(registro);
	}
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
	Cola< std::pair< std::string, std::string > > *nuevos, 
	Cola< std::pair< std::string, Lista<int> > > *modificados, 
	Cola< std::string > *eliminados){
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

	// Eliminamos posible registro temporal basura
	remove(regTmpNombre.c_str());

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
	std::string buffer;
	bool eof = false;

	// Tomamos el primer registro
	if(!std::getline(registro, buffer)) eof = true;
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
			nuevos->push(std::make_pair(ld[i], hashNuevo));

			huboCambio = true;
			continue;
		}

		// Caso en que se han eliminado archivos
		while(ld[i] > reg_archivoNombre && !eof) {
			// Insertamos en cola de eliminados
			eliminados->push(reg_archivoNombre);

			// Tomamos el registro siguiente
			buffer.clear();
			if(!std::getline(registro, buffer)) eof = true;
			this->separarNombreYHash(buffer, reg_archivoNombre, 
				reg_archivoHash);

			huboCambio = true;
		}

		// Caso en el que el archivo se mantiene existente
		if(ld[i] == reg_archivoNombre) {
			// Corroboramos si ha sido modificado
			std::string hash_aux;
			this->obtenerHash(reg_archivoNombre, hash_aux);
			int cantBloques_aux = this->obtenerCantBloques(reg_archivoNombre);
			Lista<int> listaDiferencias;

			// Caso en que el archivo ha sido modificado
			if(this->obtenerDiferencias(reg_archivoHash, 
				hash_aux, cantBloques_aux, &listaDiferencias)) {
				
				// Actualizamos el hash del archivo
				registroTmp << reg_archivoNombre << DELIMITADOR << 
					hash_aux << std::endl;


				// DEBUG
				std::cout << "HASH: " << hash_aux << std::endl;
				// END DEBUG

				// Insertamos archivo en cola de modificados
				modificados->push(make_pair(reg_archivoNombre, 
					listaDiferencias));

				huboCambio = true;
			}
			// Caso en que no ha sido modificado
			else {
				registroTmp << reg_archivoNombre << DELIMITADOR 
					<< hash_aux << std::endl;
			}

			// Tomamos el registro siguiente
			buffer.clear();
			if(!std::getline(registro, buffer)) eof = true;
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
			nuevos->push(std::make_pair(ld[i], hashNuevo));

			huboCambio = true;
		}
	}

	// Encolamos los últimos registros pertenecientes a archivos eliminados
	while(!eof) {
		eliminados->push(reg_archivoNombre);

		// Tomamos el registro siguiente
		buffer.clear();
		if(!std::getline(registro, buffer)) eof = true;
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
// PRE: las listas corresponden a que archivos nuevos o modificados deben
// tenerse en cuenta, siendo que los demás detectados en el momento de la
// actualización, son salteados.
// POST: se devuelve 'false' si se produjeron cambios en el registro o
// 'true' en su defecto; esto evita tener que revisar las colas para
// comprobar cambios.
bool ManejadorDeArchivos::actualizarRegistroDeArchivos(
	Lista< std::string >& nuevosActualizables, 
	Lista< std::string >& modificadosActualizables) {
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

	// Eliminamos posible registro temporal basura
	remove(regTmpNombre.c_str());

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
	std::string buffer;
	bool eof = false;

	// Tomamos el primer registro
	if(!std::getline(registro, buffer)) eof = true;
	this->separarNombreYHash(buffer, reg_archivoNombre, reg_archivoHash);

	// Iteramos sobre los nombres de archivos existentes en el directorio
	for(size_t i = 0; i < ld.tamanio(); i++) {
		// Caso en el que no hay mas registros y se han agregado archivos
		if(eof) {
			// Si no está en la lista de nuevos actualizables salteamos
			if(!nuevosActualizables.buscar(ld[i]))
				continue;

			// Calculamos el hash del archivo nuevo
			std::string hashNuevo;
			this->obtenerHash(ld[i], hashNuevo);

			// Registramos archivo nuevo
			registroTmp << ld[i] << DELIMITADOR << hashNuevo << std::endl;

			huboCambio = true;
			continue;
		}

		// Caso en que se han eliminado archivos
		while(ld[i] > reg_archivoNombre && !eof) {
			// Tomamos el registro siguiente
			buffer.clear();
			if(!std::getline(registro, buffer)) eof = true;
			this->separarNombreYHash(buffer, reg_archivoNombre, 
				reg_archivoHash);

			huboCambio = true;
		}

		// Caso en el que el archivo se mantiene existente
		if(ld[i] == reg_archivoNombre) {
			// Corroboramos si ha sido modificado
			std::string hash_aux;
			this->obtenerHash(reg_archivoNombre, hash_aux);
			int cantBloques_aux = this->obtenerCantBloques(reg_archivoNombre);
			Lista<int> listaDiferencias;

			// Caso en que el archivo ha sido modificado
			if(modificadosActualizables.buscar(ld[i]) && 
				this->obtenerDiferencias(reg_archivoHash, hash_aux, 
					cantBloques_aux, &listaDiferencias)) {
				// Actualizamos el hash del archivo
				registroTmp << reg_archivoNombre << DELIMITADOR << 
					hash_aux << std::endl;

				huboCambio = true;
			}
			// Caso en que no ha sido modificado
			else {
				registroTmp << reg_archivoNombre << DELIMITADOR 
					<< hash_aux << std::endl;
			}

			// Tomamos el registro siguiente
			buffer.clear();
			if(!std::getline(registro, buffer)) eof = true;
			this->separarNombreYHash(buffer, reg_archivoNombre, 
				reg_archivoHash);
		}
		// Caso en el que se han agregado nuevos archivos
		else if(ld[i] < reg_archivoNombre || eof) {
			// Si no está en la lista de nuevos actualizables salteamos
			if(!nuevosActualizables.buscar(ld[i]))
				continue;

			// Calculamos el hash del archivo nuevo
			std::string hashNuevo;
			this->obtenerHash(ld[i], hashNuevo);
			// Registramos archivo nuevo
			registroTmp << ld[i] << DELIMITADOR << hashNuevo << std::endl;

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


// Elimina el registro que identifica a un archivo en el registro de
// archivos.
// PRE: 'nombreArchivo' es el nombre del archivo a eliminar del registro.
void ManejadorDeArchivos::borrarDeRegistroDeArchivos(
	const std::string& nombreArchivo) {
	// Bloqueamos el mutex
	Lock l(m);

	// Variables auxiliares
	std::ifstream registro;
	std::ofstream registroTmp;

	// Armamos rutas de archivos
	std::string regNombre = this->directorio + "/" + DIR_AU + "/" 
		+ ARCHIVO_REG_ARCHIVOS;
	std::string regTmpNombre = this->directorio + "/" + DIR_AU + "/" 
		+ ARCHIVO_REG_ARCHIVOS + "~";

	// Eliminamos posible registro temporal basura
	remove(regTmpNombre.c_str());

	// Abrimos el registro original y el registro temporal
	registro.open(regNombre.c_str(), std::ios::in);
	registroTmp.open(regTmpNombre.c_str(), std::ios::app);

	// Verificamos si la apertura fue exitosa
	if(!registro.is_open() || !registroTmp.is_open()) 
		throw "ERROR: El registro no pudo ser abierto.";


	// Variables auxiliares de procesamiento
	std::string reg_archivoNombre, reg_archivoHash;
	std::string buffer;

	// Iteramos sobre los registros obviando el que coincide con el que
	// se desea eliminar.
	while(std::getline(registro, buffer)) {
		// Parseamos el nombre de archivo
		this->separarNombreYHash(buffer, reg_archivoNombre, reg_archivoHash);

		// Obviamos la copia si coinciden los nombres de archivo
		if(reg_archivoNombre == nombreArchivo) continue;

		// Mantenemos el registro que no se desea borrar
		registroTmp << buffer << std::endl;
	}

	// Cerramos archivos
	registro.close();
	registroTmp.close();

	// Eliminamos el registro original y convertimos el temporal en el oficial
	remove(regNombre.c_str());
	rename(regTmpNombre.c_str(), regNombre.c_str());
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


// Corrobora si se encuentra registrado un archivo en el registro de
// archivos.
// PRE: 'nombreArchivo' es el nombre de archivo.
// POST: devuelve true si el archivo se encuentra registrado o false
// en caso contrario.
bool ManejadorDeArchivos::existeArchivoEnRegitro(
	const std::string nombreArchivo) {
	// Bloqueamos el mutex
	Lock l(m);

	// Variables auxiliares
	std::ifstream registro;
	std::string reg_archivoNombre, reg_archivoHash;
	std::string buffer;

	// Armamos rutas de archivos
	std::string regNombre = this->directorio + "/" + DIR_AU + "/" 
		+ ARCHIVO_REG_ARCHIVOS;


	// Abrimos el archivo de registros
	registro.open(regNombre.c_str(), std::ios::in);

	// Verificamos si la apertura fue exitosa
	if(!registro.is_open()) 
		throw "ERROR: El registro no pudo ser abierto.";

	// Iteramos sobre las lineas del archivo para buscar el registro
	while(std::getline(registro, buffer)) {
		// Parseamos
		this->separarNombreYHash(buffer, reg_archivoNombre, reg_archivoHash);

		// Comparamos nombre de archivo del registro con el buscado
		if(reg_archivoNombre == nombreArchivo) {
			// Cerramos archivos
			registro.close();

			// Se encontró registro
			return true;
		}

		buffer.clear();
	}

	// Cerramos archivos
	registro.close();

	// No se encontró registro
	return false;
}


// Compara el hash actual de cierto bloque de archivo con un hash pasado
// por parámetro.
// PRE: 'nombreArchivo' es el nombre de archivo; 'numBloque' es el
// número del bloque que se desea comparar; 'hash' es el hash que
// se comparará con el del bloque del archivo.
// POST: devuelve true si son iguales o false si presentan diferencias.
bool ManejadorDeArchivos::compararBloque(const std::string& nombreArchivo,
	const int numBloque, const std::string& hash) {
	// Bloqueamos el mutex
	Lock l(m);

	// Variables auxiliares
	std::ifstream registro;
	std::string reg_archivoNombre, reg_archivoHash;
	std::string buffer;

	// Armamos rutas de archivos
	std::string regNombre = this->directorio + "/" + DIR_AU + "/" 
		+ ARCHIVO_REG_ARCHIVOS;


	// Abrimos el archivo de registros
	registro.open(regNombre.c_str(), std::ios::in);

	// Verificamos si la apertura fue exitosa
	if(!registro.is_open()) 
		throw "ERROR: El registro no pudo ser abierto.";

	// Iteramos sobre las lineas del archivo para buscar el registro
	while(std::getline(registro, buffer)) {
		// Parseamos
		this->separarNombreYHash(buffer, reg_archivoNombre, reg_archivoHash);

		// Comparamos nombre de archivo del registro con el buscado
		if(reg_archivoNombre == nombreArchivo) {
			bool coincide = false;

			// Corroboramos si la cantidad de bloques es menor al pasado
			if(!(this->obtenerCantBloques(nombreArchivo) < numBloque)) {
				// Extraemos el hash del bloque
				std::string hashBloque = reg_archivoHash.substr((numBloque - 1)
					* TAMANIO_BLOQUE_HASH, TAMANIO_BLOQUE_HASH);

				// Comparamos la igualdad de hashes
				if(hashBloque == hash) coincide = true;

				// DEBUG
				std::cout << hashBloque << std::endl;
				std::cout << hash << std::endl;
				// END DEBUG
			}

			// Cerramos archivos
			registro.close();

			// Se encontró registro
			return coincide;
		}

		buffer.clear();
	}

	// Cerramos archivos
	registro.close();

	// No se encontró registro
	return false;
}





/*
 * IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */


// Separa de una linea el nombre y el hash
void ManejadorDeArchivos::separarNombreYHash(const std::string &linea,
	std::string& nombre, std::string &hash) {
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
