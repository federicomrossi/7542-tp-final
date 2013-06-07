//  
//  server_carpeta.h
//  CLASE CARPETA
//  


#ifndef CARPETA_H
#define CARPETA_H


#include "common_lista.h"
#include "server_receptor.h"
class ConexionCliente;




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Carpeta {
private:

	Lista< ConexionCliente* > listaConexiones;			// Lista de clientes
	Receptor *receptor;									// Receptor
	// Sincronizador *Sincronizadordor;						// Sincronizador
	// ManejadorDeArchivos *manejadorDeArchivos;			// Manejador

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
