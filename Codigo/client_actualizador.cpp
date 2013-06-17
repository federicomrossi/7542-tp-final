//
//  client_actualizador.h
//  CLASE ACTUALIZADOR
//  


#include <sstream>
#include <utility>
#include "common_protocolo.h"
#include "common_parser.h"
#include "common_convertir.h"
#include "common_lista.h"
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
	Lista< std::string > listaArgumentos_1;
	Parser::dividirCadena(args, &listaArgumentos_1, COMMON_DELIMITER[0]);

	// Obtenemos la cantidad de archivos envió el servidor, de acuerdo al
	// protocolo
	int cantidadArchivos = Convertir::stoi(listaArgumentos_1.verPrimero());
	listaArgumentos_1.eliminarPrimero();

	// Armamos lista de pares para poder procesar en manejador de archivos
	Lista< std::pair< std::string, std::pair< std::string, int > > >
		listaServidor;

	for(int i = 0; i < cantidadArchivos; i++) {
		// Tomamos nombre de archivo
		std::string nombreArchivo = listaArgumentos_1.verPrimero();
		listaArgumentos_1.eliminarPrimero();
		// Tomamos hash de archivo
		std::string hashArchivo = listaArgumentos_1.verPrimero();
		listaArgumentos_1.eliminarPrimero();
		// Tomamos cantidad de bloques de archivo
		int cantBloquesArchivo = Convertir::stoi(listaArgumentos_1.verPrimero());
		listaArgumentos_1.eliminarPrimero();

		// Formamos el par con la información necesaria del archivo
		std::pair< std::string, int > infoArchivo;
		infoArchivo = std::make_pair(hashArchivo, cantBloquesArchivo);

		std::pair< std::string, std::pair< std::string, int > > archivo;
		archivo = std::make_pair(nombreArchivo, infoArchivo);

		listaServidor.insertarUltimo(archivo);
	}

	// Procesamos lista de archivos del servidor comparándola con el directorio
	// local y obteniendo las actualizaciones pertinentes
	Lista< std::pair< std::string, Lista< int > > > listaFaltantes;
	Lista< std::string > listaSobrantes;

	this->manejadorDeArchivos->obtenerListaDeActualizacion(&listaServidor,
		&listaFaltantes, &listaSobrantes);

	// Realizamos la petición de envío y espera de recepción de archivos
	// faltantes
	for(size_t i = 0; i < listaFaltantes.tamanio(); i++) {
		// Tomamos uno de los archivos faltantes de la lista
		std::string nombreArchivoFaltante = listaFaltantes[i].first;
		Lista< int > listaBloques = listaFaltantes[i].second;

		// Emisión de la petición de archivo
		std::string mensaje;
		mensaje.append(C_FILE_PARTS_REQUEST);
		mensaje.append(" ");
		mensaje.append(nombreArchivoFaltante);
		
		// Insertamos numeros de bloque en mensaje
		for(size_t i = 0; i < listaBloques.tamanio(); i++) {
			mensaje.append(COMMON_DELIMITER);
			mensaje.append(Convertir::itos(listaBloques[i]));
		}

		// DEBUG
		std::cout << "SOLICITO: " << mensaje << std::endl;
		// END DEBUG

		// Emitimos mensaje
		this->emisor->ingresarMensajeDeSalida(mensaje);

		std::string instr, args;
		
		// Esperamos a recibir el archivo
		while(instr != COMMON_FILE_PARTS && instr != S_NO_SUCH_FILE) {
			std::string msg = this->receptor->obtenerMensajeDeEntrada();
			Parser::parserInstruccion(msg, instr, args);
		}

		// DEBUG
		std::cout << "RECIBO: " << instr << " " << args << std::endl;
		// END DEBUG

		// Si el servidor notifica que ya no existe el archivo, salteamos
		if(instr == S_NO_SUCH_FILE) continue;


		// Parseamos los argumentos de la respuesta
		Lista< std::string > listaArgumentos_2;
		Parser::dividirCadena(args, &listaArgumentos_2, COMMON_DELIMITER[0]);

		// Descartamos el primer argumento, referido al nombre de archivo, el
		// cual es ya conocido
		std::string archivoFaltanteEntrante = listaArgumentos_2.verPrimero();
		listaArgumentos_2.eliminarPrimero();

		while(!listaArgumentos_2.estaVacia()) {
			// Tomamos un número de bloque
			int numBloque = Convertir::stoi(listaArgumentos_2.verPrimero());
			listaArgumentos_2.eliminarPrimero();
			
			// Tomamos el contenido del bloque
			std::string contenidoBloque = listaArgumentos_2.verPrimero();
			listaArgumentos_2.eliminarPrimero();

			// Caso en que estamos recibiendo un archivo entero
			if(numBloque == 0) {
				this->manejadorDeArchivos->agregarArchivo(
					nombreArchivoFaltante, contenidoBloque);

				continue;
			}

			// Modificamos el contenido del bloque por el recibido
			this->manejadorDeArchivos->modificarBloque(nombreArchivoFaltante,
				numBloque, contenidoBloque);
		}
	}

	// // Mensaje de log
	// std::cout << "Actualizando registro de archivos locales... " << std::endl;
 //   	std::cout.flush();

	// // Actualizamos el registro de archivos
	// this->manejadorDeArchivos->actualizarRegistroDeArchivos();

	// Mensaje de log
	std::cout << "Fin de la actualización... " << std::endl;
   	std::cout.flush();
}
