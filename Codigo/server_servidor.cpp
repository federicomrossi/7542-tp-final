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
Servidor::Servidor(int puerto) : puerto(puerto) {
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
	try {
		// Iniciamos la escucha del servidor
		this->socket.crear();
		this->socket.escuchar(MAX_CONEXIONES, this->puerto);
	}
	catch(char const * e) {
		// Mensaje de error
		std::cerr << e << std::endl;
		// Detenemos servidor de inmediato
		this->detener();
	}
	
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
// para iniciar. En caso de error lanza una excepción.
void Servidor::iniciar() {
	// Iniciamos hilo de ejecución
	this->start();
}


// Detiene la ejecución del servidor. No debe utilizarse el método stop()
// para detener.
void Servidor::detener() {
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
