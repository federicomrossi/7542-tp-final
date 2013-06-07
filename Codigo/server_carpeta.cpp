//  
//  server_carpeta.h
//  CLASE CARPETA
//  


#include "server_carpeta.h"
#include "server_conexion_cliente.h"



/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Carpeta::Carpeta() {
	// Creamos el receptor que recibirá los mensajes entrantes
	this->receptor = new Receptor();

	// Creamos el sincronizador
}


// Destructor
Carpeta::~Carpeta() { }


// Vincula a un cliente como miembro activo del directorio
void Carpeta::vincularCliente(ConexionCliente *unCliente) {
	// Listamos la conexión
	this->listaConexiones.insertarUltimo(unCliente);

	// Le asignamos el receptor de la carpeta para que comience a recibir
	unCliente->asignarReceptor(this->receptor);
}


// Desvincula a un cliente del directorio
void Carpeta::desvincularCliente(ConexionCliente *unCliente) {

}
