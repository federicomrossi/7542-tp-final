#include "server_administrador_de_cuentas.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "common_protocolo.h"
#include "common_parser.h"
#include "common_utilidades.h"
#include "dirent.h"

namespace {
	#define PATH_ARCHIVO "bd/Users_Pass.txt"
	#define PATH_ARCHIVO_TEMP "bd/Users_Pass_tmp.txt"
	#define LONG_PATH 10
	const std::string NOMBRE_MONITOR = "ADMMONITOR";
}

// Ctor
AdministradorDeCuentas::AdministradorDeCuentas() {
}

// Dtor
AdministradorDeCuentas::~AdministradorDeCuentas() {
}


// Devuelve la lista de usuarios que se encuentran registrados
void AdministradorDeCuentas::obtenerListaUsuarios(Lista<std::string> 
	&listaUsuarios) {

	Lock l(m);

	// Se crea el archivo si no existe, y se devuelve lista vacia
	if (!existeArchivo(PATH_ARCHIVO))
		crearArchivo(PATH_ARCHIVO);

	else {
		// Se abre el arcihvo de registros de usuario
		std::fstream archivo(PATH_ARCHIVO, std::ios_base::in |
			std::ios_base::out | std::ios_base::app);

		//Variables auxiliares
		std::string linea, nombre;

		// Se lee registro por registro guardando nombres en la lista
		while (getline(archivo, linea)) {
			nombre = linea.substr(0, linea.find( 
				COMMON_DELIMITER));
			// Si es el monitor, se saltea de la lista
			if (nombre != NOMBRE_MONITOR)
				listaUsuarios.insertarUltimo(nombre);
		}

		archivo.close();
	}
}


// Comprueba nombre de usuario y clave de los clientes
// POST: si la verificación es exitosa, se almacena en 'nombreUsuario' el
// nombre de usuario del cliente.
int AdministradorDeCuentas::verificarCliente(std::string &args, 
	std::string& nombreUsuario, std::string &pathCarpeta,
	std::string &contrasenia) {
	// Variables auxiliares
	std::string usuario, clave, carpeta;
	int delim;
	
	delim = args.find(COMMON_DELIMITER, 0);
	usuario = args.substr(0, delim);
	clave = args.substr(delim + 1, std::string::npos);
	int existe = -1;
	
	Lock l(m);

	// Si no existe, se crea el archivo
	if (!existeArchivo(PATH_ARCHIVO))
		crearArchivo(PATH_ARCHIVO);

	// Abre el archivo
	std::fstream archivo(PATH_ARCHIVO, std::ios_base::in);

	// se busca el cliente
	existe = buscarCliente(usuario, clave, carpeta);

	// Cierra el archivo
	archivo.close();

	// Si la validación es exitosa, almacenamos el nombre de usuario
	if(existe) {
		nombreUsuario = usuario;
		pathCarpeta = carpeta;
		contrasenia = clave;
	}

	return existe;
}


// Agrega un cliente a la lista
void AdministradorDeCuentas::agregarCliente(const std::string &nombre, 
	const std::string &clave) {

	Lock l(m);

	// Variables aux
	std::string carpeta;

	// Se crea un archivo si no existe
	if (!existeArchivo(PATH_ARCHIVO))
		crearArchivo(PATH_ARCHIVO);

	// Se abre el arcihvo al final
	std::fstream archivo(PATH_ARCHIVO, std::ios_base::out |
			std::ios_base::app);

	// Obtengo lista de nombres de carpeta existentes
	Lista<std::string> nombresCarpetas;
	obtenerListaCarpetas(nombresCarpetas);

	// Se crea un nombre de carpeta aleatorio y se comprueba
	// que no exista
	Utilidades::randomString(LONG_PATH, carpeta);

	// Si existe el archivo, se buscan colisiones
	if (!nombresCarpetas.estaVacia()) {
		while (nombresCarpetas.buscar(carpeta)) {
			// Se crea un nombre de carpeta aleatorio y se comprueba
			// que no exista
			Utilidades::randomString(LONG_PATH, carpeta);
		}
	}

	// Se arma el path y se crea carpeta fisica 
	Configuracion conf;
	std::string path = conf.obtenerPath() + carpeta + "/";
	crearDir(path);

	// Se agrega el usuario al final
	archivo << nombre + COMMON_DELIMITER + clave +
			COMMON_DELIMITER + carpeta + "\n";
	archivo.flush();

	archivo.close();
}

// Se elimina un cliente existente y su carpeta relacionada
void AdministradorDeCuentas::eliminarCliente(const std::string &nombre) {

	Lock l(m);

	// Se abre el archivo
	if (existeArchivo(PATH_ARCHIVO)) {
		// Variables aux
		std::string linea, nombreActual, carpeta;

		// Se crea un archivo temporal
		std::fstream temp(PATH_ARCHIVO_TEMP, std::ios_base::out |
				std::ios_base::app);

		std::fstream archivo(PATH_ARCHIVO, std::ios_base::in);
	
		// Se va leyendo el archivo original y se escriben los clientes
		// Si se encuentra el buscado, se saltea
		while (getline(archivo, linea)) {
			nombreActual = linea.substr(0, linea.find( 
				COMMON_DELIMITER));
			// si no es el que hay que eliminar, se escribe
			if (nombreActual != nombre) {
				temp << linea << std::endl;
				temp.flush();
			}
			// Si lo encontre, se elimina su carpeta
			else {
				// Se obtiene la carpeta
				carpeta = linea.substr(linea.find_last_of(
					COMMON_DELIMITER));
				// Se elimina del directorio
				eliminarDir(carpeta);
			}
		}
		// Se elimina el archivo viejo y se renombra el temporal
		archivo.close();
		temp.close();
		remove(PATH_ARCHIVO);
		rename(PATH_ARCHIVO_TEMP, PATH_ARCHIVO);
	}
}

// Se modifica un cliente existente. Si esModifNombre = true, entonces
// el 'dato' es un nombre de cliente, sino si esModifNombre = false
// es una clave.
void AdministradorDeCuentas::modificarCliente(const std::string &nombre, 
	const std::string &dato, bool esModifNombre) {

	Lock l(m);

	// Se abre el archivo
	if (existeArchivo(PATH_ARCHIVO)) {
		// Variables aux
		std::string linea, nombreActual, aux;

		// Se crea un archivo temporal
		std::fstream temp(PATH_ARCHIVO_TEMP, std::ios_base::out | 
			std::ios_base::app);

		std::fstream archivo(PATH_ARCHIVO, std::ios_base::in);
	
		// Se va leyendo el archivo original y se escriben los clientes
		// Si se encuentra el buscado, se modifica y escribe
		while (getline(archivo, linea)) {
			// Se obtiene el nombre del registro recien leido
			nombreActual = linea.substr(0, linea.find( 
				COMMON_DELIMITER));
			// si no es el que hay que modificar, se escribe
			if (nombreActual != nombre)
				temp << linea << std::endl;
			// Si encontre el que hay que modificar, se modifica
			else {
				if (esModifNombre) {
					aux = dato + linea.substr(linea.find(
							COMMON_DELIMITER));
				}
				else {
					aux = nombre + COMMON_DELIMITER + dato 
					+ linea.substr(linea.find_last_of(
					COMMON_DELIMITER));
				}
				// Se guardan cambios en archivo
				temp << aux << std::endl;
			}
		}
		// Se elimina el archivo viejo y se renombra el temporal
		archivo.close();
		temp.close();
		remove(PATH_ARCHIVO);
		rename(PATH_ARCHIVO_TEMP, PATH_ARCHIVO);
	}
}



/** Implementacion de metodos privados **/



// Se crea el archivo si no existe con permisos especiales
void AdministradorDeCuentas::crearDir(const std::string &nombre) {
	if(mkdir(nombre.c_str(), S_IRWXU | S_ISVTX) == -1)
		throw "ERROR: No se pudo crear archivo.";
}

// Se elimina un directorio y su contenido
void AdministradorDeCuentas::eliminarDir(const std::string &nombre) {

	// Variables auxiliares
	DIR *dir;
	struct dirent *entrada = 0;
	unsigned char esDirectorio = 0x4;
	Configuracion conf;
	std::string directorio = conf.obtenerPath() + nombre + "/";
	std::string nombreEntrada;

	// Abrimos directorio y procesamos si fue exitosa la apertura
	if((dir = opendir (directorio.c_str())) != NULL) {
		// Iteramos sobre cada objeto del directorio
		while ((entrada = readdir (dir)) != NULL) {
			// Se lee una entrada
			nombreEntrada = entrada->d_name;
			if (nombreEntrada != "." && nombreEntrada != "..") {
				// Si no es directorio, se elimina
				if (entrada->d_type != esDirectorio) {
					nombreEntrada = directorio + 
						entrada->d_name;
					remove(nombreEntrada.c_str());
				}
			}
		}
		closedir(dir);
		remove(directorio.c_str());
	}
}

void AdministradorDeCuentas::crearArchivo(const std::string &nombre) {
	std::fstream arch(nombre.c_str(), std::ios_base::out);
	arch.close();
	chmod(nombre.c_str(), S_IRWXU | S_ISVTX);
}

// Verifica si el archivo existe
bool AdministradorDeCuentas::existeArchivo(const std::string &nombre) {
	std::fstream arch(nombre.c_str(), std::ios_base::in);
	if (!arch.is_open())
		return false;
	arch.close();
	return true;
}

// Busqueda secuencial. Devuelve 1 si encuentra clave y usuario 
// correctamente, 2 si encuentra usuario y no clave correcta,
// y -1 si no lo encuentra
int AdministradorDeCuentas::buscarCliente(std::string &usuario, std::string &clave, 
	std::string &carpeta) {
	// Variables auxiliares
	std::string usuarioActual, claveActual, lineaActual;
	int codigo = -1;
	Lista<std::string> param;

	// Va al principio del archivo
	std::fstream archivo(PATH_ARCHIVO, std::ios_base::in);
	archivo.seekg(0, std::ios_base::beg);

	// Busca uno por uno si encuentra al cliente
	while (getline(archivo, lineaActual)) {
		// Se parsea la linea
		Parser::dividirCadena(lineaActual, &param, COMMON_DELIMITER[0]);
		// Se obtienen los datos
		usuarioActual = param[0];
		claveActual = param[1];
		// Compara
		if (usuario.compare(usuarioActual) == 0) {
			if (clave.compare(claveActual) == 0) {
				if (param.tamanio() > 2)
					carpeta = param[2];
				codigo = 1;  // Encontre usuario y clave
				break;
			}
			codigo = 2;  // Encontre usuario y no clave
			break;
		}
		param.vaciar();
	}
	return codigo;  // No encontre ni usuario ni clave
}


// Devuelve la lista de carpetas existentes en el directorio
void AdministradorDeCuentas::obtenerListaCarpetas(Lista<std::string>
	&listaCarpetas) {

	// Se crea el archivo si no existe, y se devuelve lista vacia
	if (!existeArchivo(PATH_ARCHIVO))
		crearArchivo(PATH_ARCHIVO);

	else {
		// Se abre el arcihvo de registros de usuario
		std::fstream archivo(PATH_ARCHIVO, std::ios_base::in |
			std::ios_base::out | std::ios_base::app);

		//Variables auxiliares
		std::string linea, carpeta;

		// Se lee registro por registro guardando nombres en la lista
		while (getline(archivo, linea)) {
			carpeta = linea.substr(linea.find_last_of(
				COMMON_DELIMITER), std::string::npos);
			listaCarpetas.insertarUltimo(carpeta);
		}
	}
}
