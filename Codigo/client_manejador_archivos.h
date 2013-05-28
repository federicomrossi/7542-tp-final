//
//  client_manejador_archivos.h
//  CLASE MANEJADORARCHIVOS
//  


#ifndef MANEJADOR_ARCHIVOS_H
#define MANEJADOR_ARCHIVOS_H


#include <string>
#include <list>



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ManejadorArchivos {
private:

	std::string directorio;

public:

	// Constructor
	ManejadorArchivos(const std::string& directorio);

	// Destructor
	~ManejadorArchivos();

	// Devuelve el contenido de un archivo en formato hexadecimal expresado
	// en una cadena de caracteres
	std::string obtenerContenidoArchivo(const std::string& nombre_archivo);

	// Devuelve una lista con los nombre de archivos que se encuentran ubicados
	// en el directorio que se encuentra administrando el manejador.
	std::list<std::string> obtenerArchivosDeDirectorio(); 


};

#endif
