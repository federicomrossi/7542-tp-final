//  
//  server_administrador_de_clientes.cpp
//  CLASE ADMINISTRADORDECLIENTES
//  


#include "server_administrador_de_clientes.h"

// DEBUG
#include <iostream>
// END DEBUG



/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
AdministradorDeClientes::AdministradorDeClientes() { }


// Destructor
AdministradorDeClientes::~AdministradorDeClientes() { }


// Ingresa un cliente como miembro activo del directorio al que se
// encuentra vinculado.
void AdministradorDeClientes::ingresarCliente(std::string usuario,
		ConexionCliente *unCliente) {
	// Corroboramos si ya hay una carpeta activa para dicho usuario
	// Si no existe una carpeta activa, creamos una carpeta
	if(this->carpetas.count(usuario) == 0)
		this->carpetas[usuario] = new Carpeta();

	// Vinculamos al cliente con la carpeta
	this->carpetas[usuario]->vincularCliente(unCliente);
}


// Da de baja a un cliente, el cual debe haber sido ingresado previamente
// como miembro activo del directorio al que se encuentra vinculado.
void AdministradorDeClientes::darDeBajaCliente(std::string usuario,
		ConexionCliente *unCliente) {
	// Si no hay una carpeta vinculada al usuario, no hacemos nada
	if(this->carpetas.count(usuario) == 0) return;

	// Desvinculamos la conexión de la carpeta
	this->carpetas[usuario]->desvincularCliente(unCliente);

	// Liberamos la memoria utilizada por la conexión del cliente
	delete unCliente;

	// Si la carpeta no contiene mas clientes activos, la destruimos
	if(this->carpetas[usuario]->cantidadClientes() == 0) {
		// Liberamos espacio usado por la carpeta
		delete this->carpetas[usuario];
		// Quitamos el registro del contenedor de carpetas
		this->carpetas.erase(usuario);
	}
}
