//  
//  server_emisor.h
//  CLASE EMISOR
//  


#ifndef EMISOR_H
#define EMISOR_H


#include <string>
#include <utility>
#include "common_cola.h"
#include "common_lista.h"
#include "common_thread.h"
#include "common_mutex.h"
#include "common_lock.h"
#include "common_logger.h"
#include "server_conexion_cliente.h"
#include "common_seguridad.h"





/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Emisor : public Thread {
private:
	
	// Cola de salida
	Cola< std::pair< std::string, std::pair< int, int > > > salida;	
		
	Lista< ConexionCliente* > *listaConexiones;			// Conexiones
	Mutex m;											// Mutex
	Logger *logger;										// Logger de eventos
	std::string clave;									// Clave utilizada 
														// para firmar mensajes

public:

	// Constructor
	// PRE: 'listaConexiones' es la lista de conexiones de clientes sobre las
	// que se realizan las emisiones.
	Emisor(Lista< ConexionCliente* > *listaConexiones, Logger *logger, 
		const std::string &clave);

	// Destructor
	~Emisor();

	// Inicia la emisión
	void iniciar();

	// Detiene la emisión
	void detener();

	// Ingresa un mensaje de entrada en el receptor
	// PRE: 'id' es el identificador de a quien se envía el mensaje; 'msg' es 
	// la cadena que contiene el mensaje de entrada; 'idExclusion' es el id
	// de quien debe excluirse del envío. Este último es útil cuando el id=0
	// ya que permite obviar el envio de una de todas las conexiones.
	void ingresarMensajeDeSalida(int id, std::string msg, int idExclusion);

	// Define tareas a ejecutar en el hilo.
	// Se encarga de emitir lo que se encuentre en la cola de salida.
	virtual void run();
};

#endif
