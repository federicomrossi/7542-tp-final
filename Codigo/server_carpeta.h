//  
//  server_carpeta.h
//  CLASE CARPETA
//  


#ifndef CARPETA_H
#define CARPETA_H


#include <list>
#include "server_conexion_cliente.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Carpeta {
private:

	std::list< ConexionCliente* > listaConexiones;		// Lista de clientes
	// Sincronizador sincronizador;						// Sincronizador
	// Receptor receptor;									// Receptor
	// ManejadorDeArchivos manejadorDeArchivos;			// Manejador

public:

	// Constructor
	Carpeta();

	// Destructor
	~Carpeta();

	// Vincula a un cliente como miembro activo del directorio
	void vincularCliente(ConexionCliente *unCliente);

	// Desvincula a un cliente del directorio
	void desvincularCliente(ConexionCliente *unCliente);
};

#endif
