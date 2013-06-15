//
//  server_servidor.cpp
//  CLASE SERVIDOR
//  


#include <iostream>
#include "server_servidor.h"
#include "common_lock.h"



// Constantes
namespace {
	const int MAX_CONEXIONES = 10;
}



/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Servidor::Servidor() : activo(false) {
	// Creamos al administrador de clientes
	this->admClientes = new AdministradorDeClientes;
	this->admClientes->iniciar();

	// Se crea un verificador de usuario y contrasenia
	this->verificador = new Verificador;
}


// Destructor
Servidor::~Servidor() {
	// Liberamos espacio utilizado por atributos
	this->admClientes->detener();
	this->admClientes->join();
	delete this->admClientes;
	delete this->verificador;
}


// Define tareas a ejecutar en el hilo.
// Mantiene a la escucha al servidor y acepta nuevos clientes.
void Servidor::run() {
	// Nos ponemos a la espera de clientes que se conecten
	while(this->isActive()) {
		Socket *socketCLI = 0;

		// Aceptamos nuevo cliente
		socketCLI = this->socket.aceptar();
		
		// Salimos si el socket no esta activo o si se interrumpió
		// la escucha de solicitudes de conexión
		if(!this->socket.estaActivo() || !socketCLI) break;

		// Mensaje de log
		std::cout << "Se ha conectado un cliente..." << std::endl;
	    std::cout.flush();
		
		// Generamos una nueva conexión para escuchate
		ConexionCliente *conexionCLI = new ConexionCliente(socketCLI,
				this->admClientes, this->verificador);

		// Damos la orden de que comience a ejecutarse el hilo del cliente.
		conexionCLI->start();
	}
}


// Inicia la ejecución del servidor. No debe utilizarse el método start()
// para iniciar.
// POST: si se inició correctamente el servidor devuelve true, y en caso
// contrario devuelve false
bool Servidor::iniciar(int puerto) {
	// Guardamos el puerto
	this->puerto = puerto;

	try {
		// Iniciamos la escucha del servidor
		this->socket.crear();
		this->socket.escuchar(MAX_CONEXIONES, this->puerto);
	}
	catch(char const * e) {
		// Detenemos servidor de inmediato
		this->detener();

		// Creamos entrada en Log para informar error
		std::cout << e << std::endl;
		std::cout << "ERROR: Falló el intento de conexión del servidor." 
			<< std::endl;

		return false;
	}

	// Iniciamos hilo de ejecución
	this->start();

	// Cambiamos el estado del servidor
	this->activo = true;

	return true;
}


// Detiene la ejecución del servidor. No debe utilizarse el método stop()
// para detener.
void Servidor::detener() {
	// Cambiamos el estado del servidor
	this->activo = false;

	// Detenemos hilo
	this->stop();

	// Forzamos el cierre del socket para evitar nuevas conexiones entrantes
	try {
		this->socket.cerrar();
	}
	// Ante una eventual detención abrupta, previa a la inicialización del
	// socket, lanzará un error que daremos por obviado.
	catch(...) { }
}


// Comprueba si el servidor se encuentra activo.
// POST: devuelve true si el servidor se encuentra iniciado y en ejecución
// o false si se encuentra detenido.
bool Servidor::estaActivo() {
	return this->activo;
}
