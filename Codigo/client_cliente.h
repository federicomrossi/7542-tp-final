//
//  client_cliente.h
//  CLASE CLIENTE
//  


#ifndef CLIENTE_H
#define CLIENTE_H


#include "common_socket.h"
class Comunicador;
class Logger;

#include "client_emisor.h"
#include "client_receptor.h"
#include "client_manejador_de_archivos.h"
#include "client_sincronizador.h"
#include "client_receptor_de_archivos.h"
#include "client_inspector.h"
#include "client_manejador_de_notificaciones.h"



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Cliente {
private:

	// Atributos generales
	Socket *socket;					// Socket con el que se comunica
	int puerto;						// Puerto de conexión.
	std::string nombreHost;			// Nombre del host de conexión
	bool estadoConexion;			// Censa si se encuentra conectado
	Logger *logger;					// Logger

	//Atributos de módulos
	Emisor *emisor;
	Receptor *receptor;
	ManejadorDeArchivos *manejadorDeArchivos;
	Sincronizador *sincronizador;
	ReceptorDeArchivos *receptorDeArchivos;
	Inspector *inspector;
	ManejadorDeNotificaciones *manejadorDeNotificaciones;


	// Inicia sesion con usuario existente
	int iniciarSesion(std::string usuario, std::string clave);


public:

	// Constructor
	Cliente(std::string nombreHost, int puerto, Logger *logger);

	// Destructor
	~Cliente();

	// Realiza la conexión inicial con el servidor.
	// PRE: 'usuario' y 'clave' son el nombre de usuario y contraseña con el 
	// que se desea conectar al servidor.
	// POST: devuelve '-1' si falló la conexión, '0' si falló el login y '1' si
	// se conectó y loggeó con éxito.
	int conectar(std::string usuario, std::string clave);

	// Se desconecta del servidor
	void desconectar();

	// Inicializa la sincronización del cliente con el servidor.
	// PRE: debe ejecutarse previamente el método conectar(). De lo contrario,
	// no se inicializará la sincronización.
	void iniciarSincronizacion();

	// Detiene la sincronización y se desconecta del servidor.
	// PRE: previamente debió haberse iniciado la sincronización.
	// POST: la conexión con el servidor finalizó. Si se desea volver a iniciar
	// la sincronización, debe realizarse la conexión nuevamente.
	void detenerSincronizacion();
};

#endif
