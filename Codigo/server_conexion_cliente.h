//
//  common_conexion_cliente.h
//  CLASE CONEXIONCLIENTE
//  


#ifndef CONEXION_CLIENTE_H
#define CONEXION_CLIENTE_H



#include "common_thread.h"
#include "common_socket.h"
#include "common_comunicador.h"
#include "server_verificador.h"
#include "server_receptor.h"
class AdministradorDeClientes;





/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ConexionCliente : public Thread {
private:

	Socket *socket;								// Socket de comunicación
	std::string nombreUsuario;					// Nombre de usuario de cliente
	AdministradorDeClientes *admClientes;		// Administrador de clientes
	Verificador* verificador;					// Verificador de loggin
	Receptor *receptor;							// Receptor a donde se envian
												// los datos que arrivan
	bool habilitarRecepcion;					// Traba para evitar recepción
												// hasta que se indique.

	// Espera inicio sesion
	int inicioSesion(Comunicador& comunicador);

public:

	// Constructor
	// PRE: 's' es un socket para la comunicación con el cliente; 'id' es 
	// número de cliente que se le ha sido asignado por el servidor; 'serv' es
	// una referencia al servidor al que pertenece la conexión.
	ConexionCliente(Socket *s, AdministradorDeClientes *adm, Verificador *v);
	// ConexionCliente(Socket *s, Verificador *v);

	// Destructor
	~ConexionCliente();

	// Devuelve el id que identifica a la conexión.
	int id();

	// Define tareas a ejecutar en el hilo.
	virtual void run();

	// Detiene la conexión con el cliente. No debe utilizarse el método stop()
	// para detener, sino este mismo en su lugar.
	void detener();

	// Devuelve el nombre de usuario con el que inicio sesión el cliente.
	// POST: si aún no ha iniciado sesión, se devuelve una cadena vacía.
	std::string getNombreUsuario();

	// Asigna un receptor a la conexión, a quien le enviará los datos que se
	// reciban del cliente.
	// PRE: 'unReceptor' es el receptor.
	// POST: la conexión comenzará a derivar los datos llegados hacia el
	// receptor.
	void asignarReceptor(Receptor *unReceptor);

	// Envia un mensaje al cliente.
	// PRE: 'mensaje' es la cadena que desea enviarse.
	// POST: lanza una excepción si el socket no se encuentra activo.
	void enviarMensaje(std::string& mensaje);
};

#endif
