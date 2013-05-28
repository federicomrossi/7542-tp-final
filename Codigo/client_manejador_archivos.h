//
//  client_manejador_archivos.h
//  CLASE MANEJADORARCHIVOS
//  


#ifndef MANEJADOR_ARCHIVOS_H
#define MANEJADOR_ARCHIVOS_H

#include <string>



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ManejadorArchivos {
private:


public:

	// Constructor
	ManejadorArchivos();

	// Destructor
	~ManejadorArchivos();

	//
	std::string obtenerArchivo(const std::string& nombre_archivo);


};

#endif
