//  
//  server_emisor.h
//  CLASE EMISOR
//  


#ifndef EMISOR_H
#define EMISOR_H


#include <string>
#include <utility>
#include "common_cola.h"
#include "common_thread.h"
#include "common_mutex.h"
#include "common_lock.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Emisor : public Thread {
private:

	Cola< std::pair < int, std::string > > salida;			// Cola de salida
	Mutex m;												// Mutex

public:

	// Constructor
	Emisor();

	// Destructor
	~Emisor();

	// Inicia la emisión
	void iniciar();

	// Detiene la emisión
	void detener();

	// Ingresa un mensaje de entrada en el receptor
	// PRE: 'id' es el identificador de quien ingresa el mensaje; 'msg' es la 
	// cadena que contiene el mensaje de entrada.
	void ingresarMensajeDeSalida(int id, std::string msg);

	// Define tareas a ejecutar en el hilo.
	// Se encarga de emitir lo que se encuentre en la cola de salida.
	virtual void run();
};

#endif
