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





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Actualizador::Actualizador(Emisor *emisor, Receptor *receptor, 
		ManejadorDeArchivos *manejadorDeArchivos, Logger *logger) : 
		emisor(emisor), receptor(receptor), 
		manejadorDeArchivos(manejadorDeArchivos), porcentajeDeActualizacion(0),
		logger(logger) { }


// Destructor
Actualizador::~Actualizador() { }


// Inicia la recepción
void Actualizador::ejecutarActualizacion() {
	// Mensaje de log
	this->logger->emitirLog("Actualizando directorio...");
	this->logger->emitirLog("Solicitando lista de archivos del servidor...");

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
	this->logger->emitirLog("Se recibió lista de archivos del servidor...");
	this->logger->emitirLog("Procesando lista de archivos...");

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

	//DEBUG
	std::cout << "Estoy armando lista server" << std::endl;
	//END DEBUG

	for(int i = 0; i < cantidadArchivos; i++) {
		// Tomamos nombre de archivo
		std::string nombreArchivo = listaArgumentos_1.verPrimero();
		listaArgumentos_1.eliminarPrimero();
		// Tomamos hash de archivo
		std::string hashArchivo = listaArgumentos_1.verPrimero();
		listaArgumentos_1.eliminarPrimero();
		// Tomamos cantidad de bloques de archivo
		int cantBloquesArchivo;
		cantBloquesArchivo = Convertir::stoi(listaArgumentos_1.verPrimero());
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

	//DEBUG
	std::cout << "termine con lista, voy a pedir lista actualizacion" << std::endl;
	//DEBUG

	this->manejadorDeArchivos->obtenerListaDeActualizacion(&listaServidor,
		&listaFaltantes, &listaSobrantes);


	// Eliminamos archivos sobrantes
	for(size_t i = 0; i < listaSobrantes.tamanio(); i++) {
		std::string archivo = listaSobrantes[i];
		this->manejadorDeArchivos->eliminarArchivo(archivo);
	}
	//DEBUG
	std::cout << "Termine de eliminar sobrantes" << std::endl;
	//DEBUG

	// Se crea una lista de nuevos archivos y otra de modificados
	// en el server
	Lista<std::string> nuevosActualizables;
	Lista<std::string> modificadosActualizables;

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

		// Mensaje de log
		this->logger->emitirLog("Se solicitaron archivos y partes faltantes.");

		// Emitimos mensaje
		this->emisor->ingresarMensajeDeSalida(mensaje);

		std::string instr, args;
		
		// Esperamos a recibir el archivo
		while(instr != COMMON_FILE_PARTS && instr != S_NO_SUCH_FILE) {
			std::string msg = this->receptor->obtenerMensajeDeEntrada();
			Parser::parserInstruccion(msg, instr, args);
		}

		// Mensaje de log
		this->logger->emitirLog("Se recibieron archivos y partes faltantes.");

		// Si el servidor notifica que ya no existe el archivo, salteamos
		if(instr == S_NO_SUCH_FILE) continue;


		// Parseamos los argumentos de la respuesta
		Lista< std::string > listaArgumentos_2;
		Parser::dividirCadena(args, &listaArgumentos_2, COMMON_DELIMITER[0]);

		// Descartamos el primer argumento, referido al nombre de archivo, el
		// cual es ya conocido
		std::string archivoFaltanteEntrante = listaArgumentos_2.verPrimero();
		listaArgumentos_2.eliminarPrimero();

		// Tomamos la cantidad total de bytes del archivo
		unsigned int cantTotalBytes;
		cantTotalBytes = Convertir::stoui(listaArgumentos_2.verPrimero());
		listaArgumentos_2.eliminarPrimero();

		// Creamos lista de bloques a reemplazar
		Lista< std::pair< int, std::string > > listaBloquesAReemplazar;

		// LLenamos la lista de bloques a reemplazar
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

				// Se agrega a lista de nuevos
				nuevosActualizables.insertarUltimo(
					archivoFaltanteEntrante);

				continue;
			}

			// Insertamos bloque en lista de bloques
			listaBloquesAReemplazar.insertarUltimo(std::make_pair(numBloque, 
				contenidoBloque));

			// Se agrega a lista de modificados
			modificadosActualizables.insertarUltimo(
				archivoFaltanteEntrante);
		}

		// Si la lista de bloques esta vacía, salteamos
		if(listaBloquesAReemplazar.estaVacia()) continue;

		// Modificamos el archivo
		this->manejadorDeArchivos->modificarArchivo(nombreArchivoFaltante,
			cantTotalBytes, listaBloquesAReemplazar);
	}

	// Mensaje de log
	this->logger->emitirLog("Actualizando registro de archivos locales...");

	// Actualizamos el registro de archivos
	this->manejadorDeArchivos->actualizarRegistroDeArchivos(
		nuevosActualizables, modificadosActualizables);

	// Mensaje de log
	this->logger->emitirLog("Finalizada la actualización de archivos.");
}
