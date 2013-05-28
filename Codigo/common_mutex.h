//  
//  common_mutex.h
//  CLASE MUTEX
//  
//  Clase que implementa el tipo de objetos mutex, es decir, objetos con dos
//  estados posibles: tomado y liberado. Este puede ser manipulado desde
//  varios hilos simultáneamente.
//  


#ifndef MUTEX_H
#define MUTEX_H


#include <pthread.h>




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Mutex {
private:
	
	pthread_mutex_t mutex;			// Mutex
	pthread_cond_t cond_var;		// Condition variable

	// Constructor privado
	Mutex(const Mutex &c);

	// Bloquea la ejecución en un una condition variable hasta que se produzca
	// una señalización.
	void wait();

	// Desbloquea al menos uno de los hilos que están bloqueados en la
	// condition variable.
	void signal();

	// Desbloquea todos los hilos bloqueados actualmente en la condition
	// variable.
	void broadcast();

	// Bloquea el mutex
	void lock();

	// Desbloquea el mutex
	void unlock();

public:

	// Constructor
	Mutex();

	// Destructor
	~Mutex();

	friend class Lock;
};

#endif
