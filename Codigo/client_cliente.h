//
//  client_cliente.h
//  CLASE CLIENTE
//  


#ifndef CLIENTE_H
#define CLIENTE_H


#include "common_socket.h"
#include "common_manejador_de_archivos.h"
#include "client_emisor.h"
#include "client_receptor.h"
#include "client_sincronizador.h"
#include "client_receptor_de_archivos.h"
#include "client_inspector.h"
#include "client_manejador_de_notificaciones.h"
#include "common_protocolo.h"
class Comunicador;




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Cliente {
private:

	// Atributos generales
	Socket *socket;					// Socket con el que se comunica
	int puerto;						// Puerto de conexión.
	std::string nombreHost;			// Nombre del host de conexión
	std::string directorio;			// Directorio que será sincronizado
	bool estadoConexion;			// Censa si se encuentra conectado
	bool actualizando;				// Censa si se encuentra actualizando
									// el directorio.

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
	Cliente();

	// Destructor
	~Cliente();

	// Establece el nombre de host al que se conectará el cliente.
	void especificarNombreHost(std::string nombreHost);

	// Establece el puerto del host al que se conectará el cliente
	void especificarPuerto(int puerto);

	// Establece el directorio que sincronizará el cliente
	void especificarDirectorio(std::string directorio);

	// Realiza la conexión inicial con el servidor.
	// PRE: 'usuario' y 'clave' son el nombre de usuario y contraseña con el 
	// que se desea conectar al servidor. Debe haberse especificado el nombre 
	// de host, puerto y directorio.
	// POST: devuelve '-1' si falló la conexión, '0' si falló el login y '1' si
	// se conectó y loggeó con éxito.
	int conectar(std::string usuario, std::string clave);

	// Se desconecta del servidor
	void desconectar();

	// Inicializa la sincronización del cliente con el servidor.
	// PRE: debe ejecutarse previamente el método conectar() y debe haberse
	// también especificado el nombre de host, puerto y directorio. De lo 
	// contrario, no se inicializará la sincronización. 'intervaloPolling' es 
	// el intervalo de polling que se desea al inicializar la sincronización.
	void iniciarSincronizacion(int intervaloPolling);

	// Permite cambiar el intervalo de polling estando en curso la
	// sincronización.
	// PRE: debe haber sido iniciada la sincronización. 'intervalo' es el
	// intervalo de polling expresado en segundos.
	void cambiarIntervaloPolling(unsigned int intervalo);

	// Detiene la sincronización y se desconecta del servidor.
	// PRE: previamente debió haberse iniciado la sincronización.
	// POST: la conexión con el servidor finalizó. Si se desea volver a iniciar
	// la sincronización, debe realizarse la conexión nuevamente.
	void detenerSincronizacion();

	// Comprueba si se encuentra realizando la actualización inicial
	// que se inicia al invocar al metodo iniciarSincronizacion().
	// POST: devuelve true si se encuentra actualizando o false en
	// caso contrario.
	bool estaActualizando();
};

#endif
