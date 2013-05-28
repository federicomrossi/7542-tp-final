//  
//  common_lock.h
//  CLASE LOCK
//  
//  Clase que implementa el bloqueador del mutex.
//  


#ifndef LOCK_H
#define LOCK_H


#include "common_mutex.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Lock {
private:
	
	Mutex &mutex;				// Mutex

	// Constructor privado
	Lock(const Lock &c);

public:

	// Constructor
	explicit Lock(Mutex &m);

	// Destructor
	~Lock();

	// Bloquea el mutex;
	void lock();

	// Desbloquea el mutex;
	void unlock();

	// Bloquea la ejecución en un una condition variable hasta que se produzca
	// una señalización.
	void wait();

	// Desbloquea al menos uno de los hilos que están bloqueados en la
	// condition variable.
	void signal();

	// Desbloquea todos los hilos bloqueados actualmente en la condition
	// variable.
	void broadcast();
};

#endif
