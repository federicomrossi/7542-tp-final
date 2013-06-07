//  
//  server_servidor.h
//  CLASE SERVIDOR
//  


#ifndef SERVIDOR_H
#define SERVIDOR_H


#include "common_thread.h"
#include "common_socket.h"
#include "common_lista.h"
#include "server_conexion_cliente.h"
#include "server_administrador_de_clientes.h"
#include "server_verificador.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Servidor : public Thread {
private:

	// Lista<ConexionCliente*> *clientes;			// Lista de clientes conectados
	int puerto;									// Puerto en el que se escucha.
	Socket socket;								// Socket en el que escucha el 
												// servidor.
	AdministradorDeClientes *admClientes;		// Administrador de clientes
	Verificador* verificador;					// Chequea el login de los
												// usuarios
	
	
	// Cierra todas las conexiones existentes con clientes y elimina todo 
	// registro de estos,quedando vacía la lista de clientes.
	// void cerrarConexionesConClientes();

public:

	// Constructor
	Servidor(int puerto);

	// Destructor
	~Servidor();

	// Define tareas a ejecutar en el hilo.
	// Mantiene a la escucha al servidor y acepta nuevos clientes.
	virtual void run();

	// Inicia la ejecución del servidor. No debe utilizarse el método start()
	// para iniciar. En caso de error lanza una excepción.
	void iniciar();

	// Detiene la ejecución del servidor. No debe utilizarse el método stop()
	// para detener.
	void detener();
};

#endif
