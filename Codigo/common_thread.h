//  
//  common_thread.h
//  CLASE THREAD
//  
//  Clase que implementa la interfaz para la creación de un hilo de ejecución.
//  


#ifndef THREAD_H
#define THREAD_H


#include <pthread.h>




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Thread {
private:
	
	pthread_t thread;		// Identificador del hilo
	bool status;			// Estado del thread

	// Ejecuta el método run().
	// PRE: 'threadID' es un puntero al thread.
	static void* callback(void *threadID);

	// Constructor privado
	Thread(const Thread &c);

public:

	// Constructor
	Thread();

	// Destructor
	virtual ~Thread();

	// Inicia el thread
	virtual void start();

	// Detiene el thread
	virtual void stop();

	// Envía una solicitud de cancelación al hilo, deteniendo abruptamente
	// su ejecución
	virtual void cancel();

	// Bloquea hasta que el hilo finalice su ejecución en caso de estar
	// ejecutandose.
	virtual void join();

	// Define tareas a ejecutar en el hilo.
	virtual void run() = 0;

	// Verifica si el hilo se encuentra activo.
	// POST: devuelve true si está activo o false en caso contrario.
	bool isActive();
};

#endif
