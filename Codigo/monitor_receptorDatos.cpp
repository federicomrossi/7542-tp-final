//
//  monitor_receptorDatos.cpp
//  CLASE RECEPTOR
//   


#include <iostream>
#include <sstream>

#include "common_parser.h"
#include "common_convertir.h"
#include "monitor_receptorDatos.h"

/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Receptor::Receptor() : estadoConexion(false) { }


// Destructor
Receptor::~Receptor() {
	// Liberamos la memoria utilizada por el socket
	delete this->socket;
}


// Establece el nombre de host al que se conectará el cliente.
void Receptor::especificarNombreHost(std::string nombreHost) {
	this->nombreHost = nombreHost;
}


// Establece el puerto del host al que se conectará el cliente
void Receptor::especificarPuerto(int puerto) {
	this->puerto = puerto;
}

void Receptor::especificarTiempo(int timer){
	this->timer = timer;
}
//no hayy deja de buscar recibir xq no hay pera q lo agarro de un lugar q creo q esta hecho...
// Realiza la conexión inicial con el servidor.
// PRE: 'usuario' y 'clave' son el nombre de usuario y contraseña con el 
// que se desea conectar al servidor. Debe haberse especificado el nombre 
// de host, puerto y directorio.
// POST: devuelve '-1' si falló la conexión, '0' si falló el login y '1' si
// se conectó y loggeó con éxito.
int Receptor::conectar(std::string usuario, std::string clave) {
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
		// Mensaje de logjando
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
		this->start();

		return 1;
	}

	// Destruimos el socket en caso de fallar el inicio de sesión
	desconectar();
	delete this->socket;

	// Falló el ĺoggin
	return 0;
}

// Detiene la conexión con el monitor. No debe utilizarse el método stop()
// para detener.

void Receptor::detener() {
	// Detenemos hilo
	this->stop();
	this->estadoConexion = false;
	
	// Forzamos el cierre del socket y destrabamos espera de recepcion de datos
	try {
		this->socket->cerrar();
	}
	// Ante una eventual detención abrupta, previa a la inicialización del
	// socket, lanzará un error que daremos por obviado.
	catch(...) { } 
}

bool Receptor::getEstadoConexion() {
	return this->estadoConexion;
}
void Receptor::run() {
	// Creamos el comunicador para recibir mensajes
	Comunicador comunicador(this->socket);

	// Variables de procesamiento
	std::string mensaje;
	std::string instruccion;
	std::string args;
	this->estadoConexion = true;
	int r = 0;
	while ((r == 0 ) && (this->isActive())) {
		
		mensaje = M_SERVER_INFO_REQUEST;
		
		enviarMensaje(mensaje);
		mensaje.clear();
		// Enviamos el mensaje al servidor
		Lista <std::string> aux;	
		instruccion.clear();
		args.clear();
		r = comunicador.recibir(mensaje);
		if (r != -1 ) {
		
			Parser::parserInstruccion(mensaje, instruccion, args);
			std::cout << "imprimo argumentos del mensaje "<< args <<std::endl;
			Parser::dividirCadena(args, &aux, COMMON_DELIMITER[0]);
			this->valores = aux;
			std::cout<< "Receptor actualizando valores  "<< mensaje <<std::endl;
			mensaje.clear();
		} else {
			this->estadoConexion = false;
		}

		this->sleep(this->timer);
	}
	this->estadoConexion = false;
}

// Envia un mensaje al servidor.
// PRE: 'mensaje' es la cadena que desea enviarse.
// POST: lanza una excepción si el socket no se encuentra activo.
void Receptor::enviarMensaje(std::string& mensaje) {
	// Corroboramos que el socket esté activo
	if(!this->socket->estaActivo()) {
		throw "ERROR: No se pudo emitir mensaje al servidor.";
		this->estadoConexion = false;
	}	
	// Creamos el comunicador para enviar mensajes
	Comunicador comunicador(this->socket);

	comunicador.emitir(mensaje);
	
}
int Receptor::recibirMensaje(std::string& mensaje){
	// Creamos el comunicador para recibir mensajes
	this->stop();
	Comunicador comunicador(this->socket);

	int ret = comunicador.recibir(mensaje);
	
	return ret;  
} 


// Se desconecta del servidor
void Receptor::desconectar() {
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



// Inicia sesion con Admin existente
int Receptor::iniciarSesion(std::string usuario, std::string clave) {
	// Creamos comunicador
	Comunicador com(this->socket);

	// Mensaje de log
	std::cout << "Emitiendo solicitud de LOGIN... " << std::endl;
    std::cout.flush();
	
	// Se preparan los argumentos
	std::string mensaje = usuario + COMMON_DELIMITER + clave;	

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
// actualiza los valores del monitor
Lista <std::string> Receptor::getValores() {
	Lista<std::string> destino = this->valores;

	return destino;

}

