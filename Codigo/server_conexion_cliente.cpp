//
//  common_conexion_cliente.h
//  CLASE CONEXIONCLIENTE
//   

#include <iostream>
#include <sstream>
#include "server_administrador_de_clientes.h"
#include "server_conexion_cliente.h"





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
// PRE: 's' es un socket para la comunicación con el cliente; 'id' es 
// número de cliente que se le ha sido asignado por el servidor; 'serv' es
// una referencia al servidor al que pertenece la conexión.
ConexionCliente::ConexionCliente(Socket *s,
	AdministradorDeClientes *adm, Verificador *v) : socket(s), 
	nombreUsuario(""), admClientes(adm), verificador(v), 
	habilitarRecepcion(false) { }


// Destructor
ConexionCliente::~ConexionCliente() { 
	// Liberamos memoria utilizada por el socket
	delete this->socket;
}


// Define tareas a ejecutar en el hilo.
void ConexionCliente::run() {
	// Creamos el comunicador para enviar y recibir mensajes
	Comunicador comunicador(this->socket);

	// Variables de procesamiento
	std::string mensaje;

	// Mensaje de log
	// DEBUG
	std::cout << "Esperando mensaje del cliente... ";
    std::cout.flush();
	// END DEBUG

	// Si el inicio de sesión falló, cerramos conexión con cliente
	if(this->inicioSesion(comunicador) != 1) {
		this->admClientes->destruirCliente(this);
		return;
	}

	// Nos autoregistramos en el administrador de clientes
	this->admClientes->ingresarCliente(this->nombreUsuario, this);

	// Esperamos a que se habilite la recepción, es decir, que se especifique
	// un receptor
	while(!habilitarRecepcion) 
		if(!this->isActive()) return;

	// Se inicia la recepción de mensajes desde el cliente
	while(this->isActive()) {
		// Esperamos a recibir mensaje
		if(comunicador.recibir(mensaje) == -1) break;

		// Enviamos el mensaje al receptor
		this->receptor->ingresarMensajeDeEntrada(mensaje);

		std::cout << "ENTRANTE: " <<  mensaje << std::endl;
		std::cout.flush();
	}

	// Avisamos al administrador que la conexión debe darse de baja y ser
	// destruida
	this->admClientes->darDeBajaCliente(this->nombreUsuario, this);
	this->admClientes->destruirCliente(this);
	
	// DEBUG
	std::cout << "TERMINO" << std::endl;
	// END DEBUG
}


// Detiene la conexión con el cliente. No debe utilizarse el método stop()
// para detener, sino este mismo en su lugar.
void ConexionCliente::detener() {
	// Detenemos hilo
	this->stop();
	
	// Forzamos el cierre del socket y destrabamos espera de recepcion de datos
	try {
		this->socket->cerrar();
	}
	// Ante una eventual detención abrupta, previa a la inicialización del
	// socket, lanzará un error que daremos por obviado.
	catch(...) { } 
}


// Devuelve el nombre de usuario con el que inicio sesión el cliente.
// POST: si aún no ha iniciado sesión, se devuelve una cadena vacía.
std::string ConexionCliente::getNombreUsuario() {
	return this->nombreUsuario;
}


// Asigna un receptor a la conexión, a quien le enviará los datos que se
// reciban del cliente.
// PRE: 'unReceptor' es el receptor.
// POST: la conexión comenzará a derivar los datos llegados hacia el
// receptor.
void ConexionCliente::asignarReceptor(Receptor *unReceptor) {
	this->receptor = unReceptor;

	// Habilitamos la recepción de datos
	this->habilitarRecepcion = true;
}


// Devuelve el id que identifica a la conexión.
int ConexionCliente::id() {
	//return *this->socket->id;
	return 0;
}




/*
 * IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */
 

// Espera inicio sesion
int ConexionCliente::inicioSesion(Comunicador& comunicador) {
	// Variables de procesamiento
	std::string instruccion;
	std::string args;
	
	// Se recibe la instruccion
	if(comunicador.recibir(instruccion, args) == -1) 
		return -1;

	// Se debe crear nuevo usuario
	if (instruccion == C_LOGIN_REQUEST) {
		// Caso en que la verificación es correcta
		if (verificador->verificarCliente(args, this->nombreUsuario) == 1) {  
			comunicador.emitir(S_LOGIN_OK);
			std::cout << "Inicio de sesion OK" << std::endl;
			std::cout.flush();	
			return 1;
		}
		// Caso en que la verificación es incorrecta
		else {
			comunicador.emitir(S_LOGIN_FAIL);
			std::cout << "Inicio de sesion fallo" << std::endl;
			std::cout.flush();
			return 0;
		}
	}
	return -1;
}
