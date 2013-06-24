//  
//  server_servidor.h
//  CLASE SERVIDOR
//  


#ifndef SERVIDOR_H
#define SERVIDOR_H


#include "common_thread.h"
#include "common_socket.h"
#include "common_lista.h"
#include "common_logger.h"
#include "server_conexion_cliente.h"
#include "server_administrador_de_clientes.h"
#include "server_administrador_de_cuentas.h"





/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Servidor : public Thread {
private:

	int puerto;									// Puerto en el que se escucha.
	Socket socket;								// Socket en el que escucha el 
												// servidor.
	bool activo;								// Estado del seridor
	AdministradorDeClientes *admClientes;		// Administrador de clientes
	AdministradorDeCuentas *admCuentas;					// Administra las cuentas
										// de los clientes
	Logger *logger;								// Logger de eventos

public:

	// Constructor
	Servidor();

	// Destructor
	~Servidor();

	// Define tareas a ejecutar en el hilo.
	// Mantiene a la escucha al servidor y acepta nuevos clientes.
	virtual void run();

	// Inicia la ejecución del servidor. No debe utilizarse el método start()
	// para iniciar.
	// POST: si se inició correctamente el servidor devuelve true, y en caso
	// contrario devuelve false
	bool iniciar(int puerto);

	// Detiene la ejecución del servidor. No debe utilizarse el método stop()
	// para detener.
	void detener();

	// Comprueba si el servidor se encuentra activo.
	// POST: devuelve true si el servidor se encuentra iniciado y en ejecución
	// o false si se encuentra detenido.
	bool estaActivo();
};

#endif
