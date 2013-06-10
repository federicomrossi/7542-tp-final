//
//  client_actualizador.h
//  CLASE ACTUALIZADOR
//  


#include <sstream> 
#include "common_protocolo.h"
#include "client_actualizador.h"

// DEBUG
#include <iostream>
// END DEBUG





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Actualizador::Actualizador(Emisor *emisor, Receptor *receptor, 
		ManejadorDeArchivos *manejadorDeArchivos) : emisor(emisor),
		receptor(receptor), manejadorDeArchivos(manejadorDeArchivos),
		porcentajeDeActualizacion(0) { }


// Destructor
Actualizador::~Actualizador() { }


// Inicia la recepción
void Actualizador::ejecutarActualizacion() {

	// Mensaje de log
	std::cout << "Actualizando directorio... " << std::endl;
   	std::cout.flush();
	std::cout << "Solicitando lista de archivos del servidor... " << std::endl;
   	std::cout.flush();

	// Solicitamos la lista de archivos del servidor
	this->emisor->ingresarMensajeDeSalida(C_GET_FILES_LIST);

	std::string instruccion, args;

	// Esperamos a recibir la lista de archivos desde el servidor
	while(instruccion != S_FILES_LIST) {
		std::string msg = this->receptor->obtenerMensajeDeEntrada();
		this->parserMensaje(msg, instruccion, args);
	}

	// Mensaje de log
	std::cout << "Se recibió lista de archivos del servidor... " << args << std::endl;
   	std::cout.flush();
	std::cout << "Procesando lista de archivos... " << std::endl;
   	std::cout.flush();

	// Parseamos la lista de archivos enviada por el servidor
	Lista< Archivo > listaArchivos;
	this->parserArchivos(args, &listaArchivos);

	// Procesamos lista de archivos del servidor comparándola con el directorio
	// local
	Lista< Archivo > listaArchivosFaltantes, listaArchivosParaEnviar;
	this->manejadorDeArchivos->obtenerListaDeActualizacion(&listaArchivos,
		&listaArchivosFaltantes, &listaArchivosParaEnviar);


	// Mensaje de log
	std::cout << "LISTA FALTANTES: " << listaArchivosFaltantes.tamanio() << std::endl;
	std::cout << "LISTA A ENVIAR: " << listaArchivosParaEnviar.tamanio() << std::endl;
   	std::cout.flush();

	// Realizamos la petición de envío y espera de recepción de archivos
	// faltantes
	for(size_t i = 0; i < listaArchivosFaltantes.tamanio(); i++) {
		// int obtenerArchivo(const std::string &nombre_archivo, Archivo& archivo);
		// Emisión de la petición de archivo
		std::string mensaje = C_FILE_REQUEST + " " + 
			listaArchivosFaltantes[i].obtenerNombre();
		std::cout << "PIDO: " <<  mensaje << std::endl;
		this->emisor->ingresarMensajeDeSalida(mensaje);

		std::string instruccion, args;
		
		// Esperamos a recibir el archivo
		while(instruccion != COMMON_SEND_FILE) {
			std::string msg = this->receptor->obtenerMensajeDeEntrada();
			this->parserMensaje(msg, instruccion, args);
		}

		// Parseamos el archivo
		Archivo a;
		this->parserArchivo(args, &a);

		// Almacenamos el nuevo archivo
		this->manejadorDeArchivos->agregarArchivo(a.obtenerNombre(),
			a.obtenerNumBloque(), a.obtenerBloque());
	}

	// for(size_t i = 0; i < listaArchivosParaEnviar.tamanio(); i++) {
	// 	// Emisión de la petición de archivo
	// 	std::string mensaje = COMMON_SEND_FILE + " " + 
	// 		listaArchivosFaltantes[i].obtenerNombre() + " " +
	// 		listaArchivosFaltantes[i].obtenerNumBloque() + " " +
	// 		listaArchivosFaltantes[i].obtenerBloque() + " " +
	// 		listaArchivosFaltantes[i].obtenerHash() + " " +
	// 		listaArchivosFaltantes[i].obtenerFechaDeModificacion();
	// 	this->emisor->ingresarMensajeDeSalida(mensaje);
	// }

	// Mensaje de log
	std::cout << "Fin de la actualización... " << std::endl;
   	std::cout.flush();
}




/*
 * IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */


// Parsea el mensaje separando la instruccion de sus argumentos.
// PRE: 'msg' es el mensaje que desea parsearse; 'instruccion' y 'args' son
// referencias a variables en donde se desea almacenar la instruccion y sus
// argumentos respectivamente.
void Actualizador::parserMensaje(const std::string& msg, 
	std::string& instruccion, std::string& args) {
	std::stringstream msgTemp(msg);

	// Tomamos la instrucción
	msgTemp >> instruccion;
	getline(msgTemp, args);

	// Eliminamos el espacio inicial sobrante de los argumentos
	if(args != "") args.erase(0, 1);
}


// Parsea los datos de un archivo
// PRE: 'args' es la cadena que contiene los datos separados por una coma: 
// [NOMBRE],[NUM_BLOQUE],[BLOQUE],[HASH],[FECHA]; 'archivo' es un puntero al 
// objeto Archivo en donde se almacenarán dichos datos.
void Actualizador::parserArchivo(const std::string args, Archivo *archivo) {
	std::stringstream argsTemp(args);

	// Variables auxiliares
	std::string nombre, numBloque, bloque, hash, fecha;

	getline(argsTemp, nombre, COMMON_FILE_PARAMETER_DELIMITER);
	getline(argsTemp, numBloque, COMMON_FILE_PARAMETER_DELIMITER);
	getline(argsTemp, bloque, COMMON_FILE_PARAMETER_DELIMITER);
	getline(argsTemp, hash, COMMON_FILE_PARAMETER_DELIMITER);
	getline(argsTemp, fecha, COMMON_FILE_PARAMETER_DELIMITER);

	archivo->asignarNombre(nombre);
	archivo->asignarNumBloque(numBloque);
	archivo->asignarBloque(bloque);
	archivo->asignarHash(hash);
	archivo->asignarFechaDeModificacion(fecha);
}


// Parsea la lista de archivos.
// PRE: 'listaDeArchivos' es una lista que contiene datos de archivos 
// agrupados en orden: nombre, hash, fecha; 'lista' es una lista de Archivos
// en la que se almacenarán los objetos de tipo Archivo producto del parseo.
void Actualizador::parserArchivos(const std::string& listaDeArchivos, 
	Lista< Archivo > *lista) {
	std::stringstream listaTemp(listaDeArchivos);

	// Variables auxiliares
	std::string nombre, hash, fecha;
	
	// Creamos archivo y lo insertamos en la lista
	while(listaTemp >> nombre >> hash >> fecha) {
		Archivo a(nombre);
		a.asignarHash(hash);
		a.asignarFechaDeModificacion(fecha);
		lista->insertarUltimo(a);
	}
}



