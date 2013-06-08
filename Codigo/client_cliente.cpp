//
//  client_cliente.h
//  CLASE CLIENTE
//   


#include <iostream>
#include <sstream>

#include "common_comunicador.h"
#include "common_convertir.h"
#include "common_logger.h"

#include "client_cliente.h"






/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Cliente::Cliente(std::string nombreHost, int puerto, Logger *logger) : 
	puerto(puerto), nombreHost(nombreHost), estadoConexion(false),
	logger(logger) { }


// Destructor
Cliente::~Cliente() {
	// Liberamos la memoria utilizada por el socket
	delete this->socket;
}


// Realiza la conexión inicial con el servidor.
// PRE: 'usuario' y 'clave' son el nombre de usuario y contraseña con el 
// que se desea conectar al servidor.
// POST: devuelve '-1' si falló la conexión, '0' si falló el login y '1' si
// se conectó y loggeó con éxito.
int Cliente::conectar(std::string& usuario, std::string& clave) {
	// Creamos socket
	this->socket = new Socket();
	this->socket->crear();

	// Mensaje de log
	std::cout << "Conectando con " << this->nombreHost << " en el puerto " 
		<< this->puerto << "... ";
    std::cout.flush();

	try {
		// Conectamos el socket
		this->socket->conectar(nombreHost, puerto);
	}
	catch(char const * e) {
		// Mensaje de log
		std::cout << "DESCONECTADO" << std::endl;
		std::cerr << e << std::endl;

		// Liberamos memoria
		delete this->socket;

		// Falló la conexión
		return -1;
	}

	// Mensaje de log
	std::cout << "CONECTADO" << std::endl;
	std::cout.flush();

	// Si se inició sesión con éxito, salimos y mantenemos socket activo
	if(iniciarSesion(usuario, clave) == 1) {
		// Cambiamos el estado de la conexión
		this->estadoConexion = true;

		return 1;
	}

	// Destruimos el socket en caso de fallar el inicio de sesión
	desconectar();
	delete this->socket;

	// Falló el ĺoggin
	return 0;
}


// Se desconecta del servidor
void Cliente::desconectar() {
	// Mensaje de log
	std::cout << "Cerrando conexión... ";
   	std::cout.flush();

	// Desconectamos el socket
	this->socket->cerrar();

	// Cambiamos el estado de la conexión
	this->estadoConexion = false;
	
	// Mensaje de log
	std::cout << "DESCONECTADO" << std::endl;
	std::cout.flush();
}


// Inicializa la sincronización del cliente con el servidor.
// PRE: debe ejecutarse previamente el método conectar(). De lo contrario,
// no se inicializará la sincronización.
void Cliente::iniciarSincronizacion() {
	// DEBUG
	// // Inicia sesion
	// // DEBE REEMPLAZARSE POR EL MODULO QUE CONECTA CON LA GUI
	// std::string usuario;
	// std::string clave;
	
	// while(true) {
	// 	// Solicitamos usuario y contraseña
	// 	std::cout << std::endl << "Usuario: ";
	// 	std::cout.flush();
	// 	getline(std::cin, usuario);
	// 	std::cout << "Contraseña: ";
	// 	std::cout.flush();
	// 	getline(std::cin, clave);
	// 	std::cout << std::endl;

	// 	// Si no se ingresa nada, volvemos a solicitar
	// 	if(usuario == "" || clave == "") continue;

	// 	// Se conecta al servidor
	// 	if(conectar(usuario, clave) == 1) break;
	// }
	// END DEBUG


	// Si la conexión no se encuentra activa, no hacemos nada
	if(!estadoConexion) return;


	// Creamos los módulos que conforman al cliente
	this->emisor = new Emisor(this->socket);
	this->receptor = new Receptor(this->socket);
	this->manejadorDeArchivos = new ManejadorDeArchivos("cliente");
	this->sincronizador = new Sincronizador(emisor);
	this->receptorDeArchivos = new ReceptorDeArchivos(manejadorDeArchivos);

	int INTERVALO = 5; // CAMBIAR POR ARCHIVO DE CONFIGURACIÓN
	this->inspector = new Inspector(manejadorDeArchivos, sincronizador,
		INTERVALO);

	this->manejadorDeNotificaciones = new ManejadorDeNotificaciones(receptor,
		sincronizador, receptorDeArchivos);


	// Ponemos en marcha los módulos
	this->receptor->iniciar();
	this->emisor->iniciar();
	this->manejadorDeNotificaciones->start();
	this->inspector->iniciar();


	// DEBUG
	// // Esperamos a que se de la indicación de finalizar el cliente
	// // DEBE REEMPLAZARSE POR MODULO QUE CONECTA CON LA GUI
	// std::string comando;
	// while(comando != "s")
	// 	getline(std::cin, comando);
	// // FIN indicación de salida

	// this->detenerSincronizacion();
	// END DEBUG
}


// Detiene la sincronización y se desconecta del servidor.
// PRE: previamente debió haberse iniciado la sincronización.
// POST: la conexión con el servidor finalizó. Si se desea volver a iniciar
// la sincronización, debe realizarse la conexión nuevamente.
void Cliente::detenerSincronizacion() {
	// Detenemos los módulos
	this->inspector->detener();
	this->inspector->join();
	this->emisor->detener();
	this->receptor->detener();
	this->manejadorDeNotificaciones->stop();
	this->manejadorDeNotificaciones->join();
	this->emisor->join();

	// Se desconecta del servidor
	this->desconectar();	
	
	this->receptor->join();

	// Liberamos la memoria utilizada por los módulos
	delete this->emisor;
	delete this->receptor;
	delete this->manejadorDeArchivos;
	delete this->sincronizador;
	delete this->receptorDeArchivos;
	delete this->inspector;
	delete this->manejadorDeNotificaciones;
}





/*
 * IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */
 

// Inicia sesion con usuario existente
int Cliente::iniciarSesion(std::string &usuario, std::string &clave) {
	// Creamos comunicador
	Comunicador com(this->socket);

	// Mensaje de log
	std::cout << "Emitiendo solicitud de LOGIN... " << std::endl;
    std::cout.flush();
	
	// Se preparan los argumentos
	std::string mensaje = usuario + '-' + clave;	

	// Enviamos petición de inicio de sesion
	if(com.emitir(C_LOGIN_REQUEST, mensaje) == -1) {
		return -1;
	}

	// Se obtiene respuesta del servidor
	std::string args;
	if(com.recibir(mensaje, args) == -1) {
		return -1;
	}
	
	if (mensaje == S_LOGIN_OK) {
		std::cout << "Inicio de sesion exitoso" << std::endl;
		std::cout.flush();
		return 1;
	}
	if (mensaje == S_LOGIN_FAIL) {
		std::cout << "Inicio de sesion fallo, compruebe nombre de usuario y contrasenia" << std::endl;
		std::cout.flush();
		return 0;
	}
	return -1;
}
