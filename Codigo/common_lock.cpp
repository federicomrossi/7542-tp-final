//  
//  common_lock.cpp
//  CLASE LOCK
//  
//  Clase que implementa el bloqueador del mutex.
//  


#include "common_lock.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Lock::Lock(Mutex &m) : mutex(m) {
	this->mutex.lock();
}


// Constructor privado
Lock::Lock(const Lock &c) : mutex(c.mutex) { }


// Destructor
Lock::~Lock() {
	this->mutex.unlock();
}


// Bloquea el mutex;
void Lock::lock() {
	this->mutex.lock();
}


// Desbloquea el mutex;
void Lock::unlock() {
	this->mutex.unlock();
}


// Bloquea la ejecución en un una condition variable hasta que se produzca
// una señalización.
void Lock::wait() {
	this->mutex.wait();
}


// Desbloquea al menos uno de los hilos que están bloqueados en la
// condition variable.
void Lock::signal() {
	this->mutex.signal();
}


// Desbloquea todos los hilos bloqueados actualmente en la condition
// variable.
void Lock::broadcast() {
	this->mutex.broadcast();
}
