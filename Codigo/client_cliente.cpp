//
//  client_cliente.h
//  CLASE CLIENTE
//   


#include <iostream>
#include <sstream>
#include "common_comunicador.h"
#include "common_convertir.h"
#include "client_config.h"
#include "client_actualizador.h"
#include "client_cliente.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Cliente::Cliente() : estadoConexion(false), actualizando(true) {
	// Creamos el logger
	this->logger = new Logger(LOGGER_RUTA_LOG + LOGGER_NOMBRE_LOG);
}


// Destructor
Cliente::~Cliente() {
	// Liberamos la memoria utilizada por el socket
	delete this->socket;
	delete this->logger;
}


// Establece el nombre de host al que se conectará el cliente.
void Cliente::especificarNombreHost(std::string nombreHost) {
	this->nombreHost = nombreHost;
}


// Establece el puerto del host al que se conectará el cliente
void Cliente::especificarPuerto(int puerto) {
	this->puerto = puerto;
}


// Establece el directorio que sincronizará el cliente
void Cliente::especificarDirectorio(std::string directorio) {
	this->directorio = directorio;
}


// Realiza la conexión inicial con el servidor.
// PRE: 'usuario' y 'clave' son el nombre de usuario y contraseña con el 
// que se desea conectar al servidor. Debe haberse especificado el nombre 
// de host, puerto y directorio.
// POST: devuelve '-1' si falló la conexión, '0' si falló el login y '1' si
// se conectó y loggeó con éxito.
int Cliente::conectar(std::string usuario, std::string clave) {
	// Creamos socket
	this->socket = new Socket();
	this->socket->crear();

	// Mensaje de log
	this->logger->emitirLog("Conectando con " + this->nombreHost +
		" en el puerto " + Convertir::itos(this->puerto));

	try {
		// Conectamos el socket
		this->socket->conectar(nombreHost, puerto);
	}
	catch(char const * e) {
		// Mensaje de log
		this->logger->emitirLog("No se ha podido conectar con servidor.");
		this->logger->emitirLog(e);

		// Liberamos memoria
		delete this->socket;

		// Falló la conexión
		return -1;
	}

	// Mensaje de log
	this->logger->emitirLog("Conexión establecida con servidor.");

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
	this->logger->emitirLog("Se ha cerrado la conexión con el servidor.");
}


// Inicializa la sincronización del cliente con el servidor.
// PRE: debe ejecutarse previamente el método conectar() y debe haberse
// también especificado el nombre de host, puerto y directorio. De lo 
// contrario, no se inicializará la sincronización. 'intervaloPolling' es 
// el intervalo de polling que se desea al inicializar la sincronización.
void Cliente::iniciarSincronizacion(int intervaloPolling) {
	// Si la conexión no se encuentra activa, no hacemos nada
	if(!estadoConexion) return;

	// Activamos flag de actualización
	this->actualizando = true;

	// Creamos los módulos primarios
	this->emisor = new Emisor(this->socket, this->logger, this->clave);
	this->receptor = new Receptor(this->socket, this->logger, this->clave);
	this->manejadorDeArchivos = new ManejadorDeArchivos(this->directorio,
		this->logger);
	
	// Ponemos en marcha los módulos
	this->receptor->iniciar();
	this->emisor->iniciar();

	// Iniciamos la actualización del directorio local
	Actualizador actualizador(this->emisor, this->receptor,
		this->manejadorDeArchivos, this->logger);
	actualizador.ejecutarActualizacion();

	// Creamos los módulos para la sincronización en tiempo real
	this->sincronizador = new Sincronizador(emisor, this->logger);
	this->receptorDeArchivos = new ReceptorDeArchivos(manejadorDeArchivos,
		this->logger);
	this->inspector = new Inspector(manejadorDeArchivos, sincronizador,
		intervaloPolling, this->logger);
	this->manejadorDeNotificaciones = new ManejadorDeNotificaciones(receptor,
		inspector, receptorDeArchivos, this->logger);

	// Activamos flag de actualización
	this->actualizando = false;

	// Ponemos en marcha los módulos
	this->inspector->iniciar();
	this->manejadorDeNotificaciones->start();
}


// Permite cambiar el intervalo de polling estando en curso la
// sincronización.
// PRE: debe haber sido iniciada la sincronización. 'intervalo' es el
// intervalo de polling expresado en segundos.
void Cliente::cambiarIntervaloPolling(unsigned int intervalo) {
	try {
		this->inspector->establecerIntervaloDeInspeccion(intervalo);
	}
	catch (...) {
		std::cerr << "ERROR: Debe inicializarse la sincronización" 
			<< std::endl;
	}
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


// Comprueba si se encuentra realizando la actualización inicial
// que se inicia al invocar al metodo iniciarSincronizacion().
// POST: devuelve true si se encuentra actualizando o false en
// caso contrario.
bool Cliente::estaActualizando() {
	return this->actualizando;
}


// Comprueba si se encuentra activa la conexión con el servidor y si se
// encuentra sincronizando.
// POST: devuelve true si se encuentra activo o false en su defecto.
bool Cliente::estaSincronizando() {
	try{
		bool e = this->emisor->emisionActiva();
		bool r = this->receptor->recepcionActiva();

		if(e || r) return true;
		else return false;
	}
	catch(...) {
		return false;
	}
}




/*
 * IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */


// Inicia sesion con usuario existente
int Cliente::iniciarSesion(std::string usuario, std::string clave) {
	// Creamos comunicador
	Comunicador com(this->socket);

	// Mensaje de log
	this->logger->emitirLog("Emitiendo solicitud de LOGIN...");
	
	// Se preparan los argumentos
	std::string hashClave = Hash::funcionDeHash(clave);
	std::string mensaje = usuario + COMMON_DELIMITER + hashClave;	

	//DEBUG
	std::cout << "Hash clave: " << hashClave << std::endl;
	//END DEBUG

	// Enviamos petición de inicio de sesion.
	if(com.emitir(C_LOGIN_REQUEST, mensaje) == -1) return -1;

	// Se obtiene respuesta del servidor
	std::string args;
	if(com.recibir(mensaje, args) == -1) return -1;
	
	if (mensaje == S_LOGIN_OK) {
		// Mensaje de log
		this->logger->emitirLog("Inicio de sesión exitoso con usuario '" +
			usuario + "'");

		// Se guarda la clave para enviar mensajes con firma
		this->clave = hashClave;

		return 1;
	}
	else if (mensaje == S_LOGIN_FAIL) {
		// Mensaje de log
		this->logger->emitirLog("Falló inicio de sesión con usuario '" +
			usuario + "'");

		return 0;
	}

	return -1;
}
