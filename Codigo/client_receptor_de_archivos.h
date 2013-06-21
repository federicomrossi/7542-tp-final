//
//  client_receptor_de_archivos.h
//  CLASE RECEPTORDEARCHIVOS
//  


#ifndef RECEPTOR_DE_ARCHIVOS_H
#define RECEPTOR_DE_ARCHIVOS_H


#include <string>
#include "common_manejador_de_archivos.h"
#include "common_protocolo.h"
class Mutex;
class Lock;



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ReceptorDeArchivos {
private:

	Mutex m;							// Mutex
	ManejadorDeArchivos *manejadorDeArchivos;		// Manejador de archivos
	
	void parsearMensaje(const std::string &mensaje, std::string &accion,
		std::string &nombre_archivo, std::string &num_bloque, std::string &bloque_archivo);

public:

	// Constructor
	ReceptorDeArchivos(ManejadorDeArchivos *unManejador);

	// Destructor
	~ReceptorDeArchivos();

	// Se encarga de procesar la recepción de un archivo nuevo.
	// PRE: 'nombreArchivo' es el nombre del archivo a recibir; 'contenido' es
	// el contenido del archivo a recibir.
	void recibirArchivo(const std::string& nombreArchivo, 
		const std::string& contenido);

	// Se encarga de procesar la eliminación de un archivo
	void eliminarArchivo(std::string& nombreArchivo);

	// Se encarga de procesar la recepción de modificaciones en archivo.
	// PRE: 'nombreArchivo' es el nombre del archivo a modificar;
	// 'cantloquesDelArchivo' es la cantidad nueva de bloques que debe
	// contener el archivo; 'listaBloquesAReemplazar' es una lista que
	// contiene los números de bloque y su respectivo contenido, los
	// cuales reemplazarán a los bloques actuales.
	void recibirModificaciones(std::string& nombreArchivo, 
		unsigned int cantBytesDelArchivo, 
		Lista< std::pair< int, std::string > >& listaBloquesAReemplazar);
};

#endif
