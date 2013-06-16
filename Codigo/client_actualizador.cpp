//
//  client_actualizador.h
//  CLASE ACTUALIZADOR
//  


#include <sstream>
#include <utility>
#include "common_protocolo.h"
#include "common_parser.h"
#include "common_convertir.h"
#include "common_cola.h"
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

   	// Creamos el registro de archivos en caso de que no exista
   	if(this->manejadorDeArchivos->crearRegistroDeArchivos());

	// Solicitamos la lista de archivos del servidor
	this->emisor->ingresarMensajeDeSalida(C_GET_FILES_LIST);

	std::string instruccion, args;

	// Esperamos a recibir la lista de archivos desde el servidor
	while(instruccion != S_FILES_LIST) {
		std::string msg = this->receptor->obtenerMensajeDeEntrada();
		Parser::parserInstruccion(msg, instruccion, args);
	}

	// Mensaje de log
	std::cout << "Se recibió lista de archivos del servidor... " << args << std::endl;
   	std::cout.flush();
	std::cout << "Procesando lista de archivos... " << std::endl;
   	std::cout.flush();

	// Parseamos la lista de archivos enviada por el servidor
	Lista< std::string > listaArgumentos;
	Parser::parserArgumentos(args, &listaArgumentos, COMMON_DELIMITER[0]);

	// Obtenemos la cantidad de archivos envió el servidor, de acuerdo al
	// protocolo
	int cantidadArchivos = Convertir::stoi(listaArgumentos.verPrimero());
	listaArgumentos.eliminarPrimero();

	// Armamos lista de pares para poder procesar en manejador de archivos
	Lista< std::pair< std::string, std::pair< std::string, int > > >
		listaServidor;

	for(int i = 0; i < cantidadArchivos; i++) {
		// Tomamos nombre de archivo
		std::string nombreArchivo = listaArgumentos.verPrimero();
		listaArgumentos.eliminarPrimero();
		// Tomamos hash de archivo
		std::string hashArchivo = listaArgumentos.verPrimero();
		listaArgumentos.eliminarPrimero();
		// Tomamos cantidad de bloques de archivo
		int cantBloquesArchivo = Convertir::stoi(listaArgumentos.verPrimero());
		listaArgumentos.eliminarPrimero();

		// Formamos el par con la información necesaria del archivo
		std::pair< std::string, int > infoArchivo;
		infoArchivo = std::make_pair(hashArchivo, cantBloquesArchivo);

		std::pair< std::string, std::pair< std::string, int > > archivo;
		archivo = std::make_pair(nombreArchivo, infoArchivo);

		listaServidor.insertarUltimo(archivo);
	}

	// Procesamos lista de archivos del servidor comparándola con el directorio
	// local y obteniendo las actualizaciones pertinentes
	Lista< std::pair< std::string, Cola< int > > > listaFaltantes;
	Lista< std::string > listaSobrantes;

	this->manejadorDeArchivos->obtenerListaDeActualizacion(&listaServidor,
		&listaFaltantes, listaSobrantes);

	// // Realizamos la petición de envío y espera de recepción de archivos
	// // faltantes
	// for(size_t i = 0; i < listaArchivosFaltantes.tamanio(); i++) {
	// 	// Emisión de la petición de archivo
	// 	std::string mensaje = C_FILE_REQUEST + " " + 
	// 		listaArchivosFaltantes[i].obtenerNombre();
	// 	this->emisor->ingresarMensajeDeSalida(mensaje);

	// 	std::string instr, args;
		
	// 	// Esperamos a recibir el archivo
	// 	while(instr != COMMON_SEND_FILE && instr != S_NO_SUCH_FILE) {
	// 		std::string msg = this->receptor->obtenerMensajeDeEntrada();
	// 		this->parserMensaje(msg, instr, args);
	// 	}


	// 	// Si el servidor notifica que ya no existe el archivo, salteamos
	// 	if(instr == S_NO_SUCH_FILE) continue;

	// 	// Parseamos el archivo
	// 	Archivo a;
	// 	this->parserArchivo(args, &a);

	// 	// Almacenamos el nuevo archivo
	// 	this->manejadorDeArchivos->agregarArchivo(a.obtenerNombre(),
	// 		WHOLE_FILE, a.obtenerBloque());
	// }

	// // Mensaje de log
	// std::cout << "Actualizando registro de archivos locales... " << std::endl;
 //   	std::cout.flush();

	// // Actualizamos el registro de archivos
	// this->manejadorDeArchivos->actualizarRegistroDeArchivos();

	// // Mensaje de log
	// std::cout << "Fin de la actualización... " << std::endl;
 //   	std::cout.flush();
}
