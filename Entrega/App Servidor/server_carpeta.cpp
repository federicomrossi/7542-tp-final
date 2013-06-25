//  
//  server_carpeta.h
//  CLASE CARPETA
//  


#include "server_config.h"
#include "server_carpeta.h"
#include "server_conexion_cliente.h"
#include <dirent.h>
#include <sys/stat.h>





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Carpeta::Carpeta(const std::string &pathCarpeta, Logger *logger, 
	const std::string &clave) : 
	logger(logger) {
	// Creamos el receptor que recibirá los mensajes entrantes
	this->receptor = new Receptor(this->logger, clave);

	// Creamos el emisor que enviará mensajes a los clientes
	this->emisor = new Emisor(&this->listaConexiones, this->logger,
		clave);

	// Si no existe carpeta fisica se crea. 
	// Si no lo logra, lanza excepcion
	std::string path = DIR_RAIZ_CARPETAS + pathCarpeta + "/";

	// Si no se pudo crear el directorio, lanzamos error
	if(!crearCarpeta(path)){
		// Mensaje de log
		this->logger->emitirLog("ERROR: No se pudo crear directorio " + path);
		throw "ERROR: No se pudo crear directorio";
	}

	// Se crea el manejador de archivos
	this->manejadorDeArchivos = new ManejadorDeArchivos(path, this->logger);

	// Creamos el sincronizador
	this->sincronizador = new Sincronizador(this->receptor, this->emisor,
		this->manejadorDeArchivos, this->logger);

	// Iniciamos los hilos
	this->emisor->iniciar();
	this->sincronizador->iniciar();
}


// Destructor
Carpeta::~Carpeta() {
	// Detenemos módulos
	this->sincronizador->detener();
	this->sincronizador->join();
	
	this->emisor->detener();
	this->emisor->join();

	// Liberamos memoria utilizada
	delete this->sincronizador;
	delete this->receptor;
	delete this->emisor;
	delete this->manejadorDeArchivos;
}


// Vincula a un cliente como miembro activo del directorio
void Carpeta::vincularCliente(ConexionCliente *unCliente) {
	// Listamos la conexión
	this->listaConexiones.insertarUltimo(unCliente);

	// Le asignamos el receptor de la carpeta para que comience a recibir
	unCliente->asignarReceptor(this->receptor);
}


// Desvincula a un cliente del directorio.
void Carpeta::desvincularCliente(ConexionCliente *unCliente) {
	this->listaConexiones.eliminar(unCliente);
}


// Devuelve la cantidad de clientes que se encuentran activos en la carpeta
int Carpeta::cantidadClientes() {
	return this->listaConexiones.tamanio();
}






/*
 * IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */


// Crea una carpeta fisica para el usuario si no existe ya una carpeta
// Devuelve 1 si la operacion es correcta y 0 sino
int Carpeta::crearCarpeta(const std::string &pathCarpeta) {
	// Se intenta abrir el directorio
	DIR* carpeta = opendir(pathCarpeta.c_str());
	if (carpeta == NULL) {  // No existe, entonces se crea
		if (!mkdir(pathCarpeta.c_str(), S_IFDIR | S_IRWXU | S_IFDIR));
			return 0;
	}
	else
		closedir(carpeta);
	return 1;
}

