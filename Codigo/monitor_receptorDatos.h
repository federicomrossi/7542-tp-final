//
//  monitor_receptorDatos.h
//  CLASE RECEPTOR
//  


#ifndef RECEPTOR_H
#define RECEPTOR_H

#include <string>
#include "common_comunicador.h"
#include "common_thread.h"
#include "common_lista.h"
#include "common_socket.h"
#include "common_protocolo.h"
class Comunicador;



/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Receptor : public Thread {
private:

	// Atributos generales
	Socket *socket;					// Socket con el que se comunica
	int puerto;						// Puerto de conexión.
	int timer;						// Timer de actualizacion de informacion
	std::string nombreHost;			// Nombre del host de conexión
	bool estadoConexion;			// Censa si se encuentra conectado
	Lista<std::string> valores;     // valores recibidos
	// Inicia sesion con usuario existente
	int iniciarSesion(std::string usuario, std::string clave);

public:

	// Constructor
	Receptor();

	// Destructor
	~Receptor();


	// Define tareas a ejecutar en el hilo.
	virtual void run();

	// Detiene la conexión con el monitor. No debe utilizarse el método stop()
	// para detener, sino este mismo en su lugar.
	void detener();


	// Establece el nombre de host al que se conectará el monitor.
	void especificarNombreHost(std::string nombreHost);

	// Establece el puerto del host al que se conectará el monitor
	void especificarPuerto(int puerto);

	// Establece el tiempo actualizacion de informacion
	void especificarTiempo(int tiempo);




	// Realiza la conexión inicial con el servidor.
	// PRE: 'usuario' y 'clave' son el nombre de usuario y contraseña con el 
	// que se desea conectar al servidor. Debe haberse especificado el nombre 
	// de host y puerto.
	// POST: devuelve '-1' si falló la conexión, '0' si falló el login y '1' si
	// se conectó y loggeó con éxito.
	int conectar(std::string usuario, std::string clave);

	// Se desconecta del servidor
	void desconectar();

	// Envia un mensaje al cliente.
	// PRE: 'mensaje' es la cadena que desea enviarse.
	// POST: lanza una excepción si el socket no se encuentra activo.
	void enviarMensaje(std::string& mensaje);

	// Retorna la ultima version de valores obtenidos del servidor
	Lista <std::string> getValores();

	// Retorna estado de conexion
	bool getEstadoConexion();

};

#endif
