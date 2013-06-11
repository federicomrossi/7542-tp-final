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
		// Emisión de la petición de archivo
		std::string mensaje = C_FILE_REQUEST + " " + 
			listaArchivosFaltantes[i].obtenerNombre();
		std::cout << "PIDO: " <<  mensaje << std::endl;
		this->emisor->ingresarMensajeDeSalida(mensaje);

		std::string instr, args;
		
		// Esperamos a recibir el archivo
		while(instr != COMMON_SEND_FILE && instr != S_NO_SUCH_FILE) {
			std::string msg = this->receptor->obtenerMensajeDeEntrada();
			this->parserMensaje(msg, instr, args);
			std::cout << "RESPONDIO ANTE PETICION: " << instr << " " << args << std::endl;
		}


		// Si el servidor notifica que ya no existe el archivo, salteamos
		if(instr == S_NO_SUCH_FILE) continue;

		// Parseamos el archivo
		Archivo a;
		this->parserArchivo(args, &a);

		// Almacenamos el nuevo archivo
		this->manejadorDeArchivos->agregarArchivo(a.obtenerNombre(),
			WHOLE_FILE, a.obtenerBloque());
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

	// Actualizamos el registro de archivos
	this->manejadorDeArchivos->actualizarRegistroDeArchivos();

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
void Actualizador::parserArchivo(const std::string argumentos, Archivo *archivo) {

	// El mensaje viene en el formato "<Nombre,Numero_Bloque,Bloque,Hash,Fecha>"
	// Divididos por una ','
	std::string args[5];
	std::string aux;
	std::string msj = argumentos;
	int i;
	int delim = 0;
	
	// Se parsea el mensaje
	for (i = 0; i < 5; i++) {
		delim = msj.find(COMMON_DELIMITER);
		aux = msj.substr(0, delim);
		msj.erase(0, delim + 1);
		args[i].assign(aux.c_str());
	}	

	archivo->asignarNombre(args[0]);
	archivo->asignarNumBloque(args[1]);
	archivo->asignarBloque(args[2]);
	archivo->asignarHash(args[3]);
	archivo->asignarFechaDeModificacion(args[4]);
}


// Parsea la lista de archivos.
// PRE: 'listaDeArchivos' es una lista que contiene datos de archivos 
// agrupados en orden: nombre, hash, fecha; 'lista' es una lista de Archivos
// en la que se almacenarán los objetos de tipo Archivo producto del parseo.
void Actualizador::parserArchivos(const std::string& listaDeArchivos, 
	Lista< Archivo > *lista) {
	
	// Variables auxiliares
	std::string args[3];
	std::string aux;
	std::string msj = listaDeArchivos;
	int i, j, cantArchivos = 0;
	int delim = 0;

	// Se busca la cantidad de archivos
	delim = msj.find(COMMON_DELIMITER);
	aux = msj.substr(0, delim);
	msj.erase(0, delim + 1);
	cantArchivos = Convertir::stoi(aux);

	// Creamos archivo y lo insertamos en la lista
	for (j = 0; j < cantArchivos; j++)  {	
		// Se parsea el mensaje
		for (i = 0; i < 3; i++) {
			delim = msj.find(COMMON_DELIMITER);
			aux = msj.substr(0, delim);
			msj.erase(0, delim + 1);
			args[i].assign(aux.c_str());
		}
		std::cout << "Mensaje parseado: " << args[0] << " " << args[1] << " " << args[2] << std::endl;
		Archivo a(args[0]);
		a.asignarHash(args[1]);
		a.asignarFechaDeModificacion(args[2]);
		lista->insertarUltimo(a);
	}
}



