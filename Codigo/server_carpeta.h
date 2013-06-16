//  
//  server_carpeta.h
//  CLASE CARPETA
//  


#ifndef CARPETA_H
#define CARPETA_H


#include "common_lista.h"
#include "server_receptor.h"
#include "server_emisor.h"
#include "server_sincronizador.h"
#include "common_manejador_de_archivos.h"
#include "server_conexion_cliente.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Carpeta {
private:

	Lista< ConexionCliente* > listaConexiones;			// Lista de clientes
	Receptor *receptor;									// Receptor
	Emisor *emisor;										// Emisor
	Sincronizador *sincronizador;						// Sincronizador
	ManejadorDeArchivos *manejadorDeArchivos;			// Manejador

public:

	// Constructor
	Carpeta();

	// Destructor
	~Carpeta();

	// Vincula a un cliente como miembro activo del directorio
	void vincularCliente(ConexionCliente *unCliente);

	// Desvincula a un cliente del directorio.
	void desvincularCliente(ConexionCliente *unCliente);

	// Devuelve la cantidad de clientes que se encuentran activos en la carpeta
	int cantidadClientes();
};

#endif
