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

	// Creamos el emisor que enviará mensajes a los clientes
	this->emisor = new Emisor(&this->listaConexiones);

	// Se crea el manejador de archivos
	// DEBUG: Modificar path
	std::string path = "servidor";
	this->manejadorDeArchivos = new ManejadorDeArchivos(path);

	// Creamos el sincronizador
	this->sincronizador = new Sincronizador(this->receptor, this->emisor,
		this->manejadorDeArchivos);

	// Iniciamos los hilos
	this->emisor->iniciar();
	this->sincronizador->iniciar();
}


// Destructor
Carpeta::~Carpeta() {
	// Detenemos módulos
	this->sincronizador->detener();
	this->sincronizador->join();
	
	this->emisor->detener();
	this->emisor->join();

	// Liberamos memoria utilizada
	delete this->sincronizador;
	delete this->receptor;
	delete this->emisor;
	delete this->manejadorDeArchivos;
}


// Vincula a un cliente como miembro activo del directorio
void Carpeta::vincularCliente(ConexionCliente *unCliente) {
	// Listamos la conexión
	this->listaConexiones.insertarUltimo(unCliente);

	// Le asignamos el receptor de la carpeta para que comience a recibir
	unCliente->asignarReceptor(this->receptor);
}


// Desvincula a un cliente del directorio.
void Carpeta::desvincularCliente(ConexionCliente *unCliente) {
	this->listaConexiones.eliminar(unCliente);
}


// Devuelve la cantidad de clientes que se encuentran activos en la carpeta
int Carpeta::cantidadClientes() {
	return this->listaConexiones.tamanio();
}
