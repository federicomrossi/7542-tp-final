//  
//  server_administrador_de_clientes.h
//  CLASE ADMINISTRADORDECLIENTES
//  


#ifndef ADMINISTRADOR_DE_CLIENTES_H
#define ADMINISTRADOR_DE_CLIENTES_H


#include <string>
#include <map>

// class ConexionCliente;
#include "server_conexion_cliente.h"
#include "server_carpeta.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class AdministradorDeClientes {
private:

	std::map< std::string, Carpeta* > carpetas;		// Diccionario de carpetas

public:

	// Constructor
	AdministradorDeClientes();

	// Destructor
	~AdministradorDeClientes();

	// Ingresa un cliente como miembro activo del directorio al que se
	// encuentra vinculado.
	void ingresarCliente(std::string usuario, ConexionCliente *unCliente);

	// Da de baja a un cliente, el cual debe haber sido ingresado previamente
	// como miembro activo del directorio al que se encuentra vinculado.
	void darDeBajaCliente(std::string usuario, ConexionCliente *unCliente);
};

#endif
