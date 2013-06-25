//  
//  common_mutex.cpp
//  CLASE MUTEX
//  
//  Clase que implementa el tipo de objetos mutex, es decir, objetos con dos
//  estados posibles: tomado y liberado. Este puede ser manipulado desde
//  varios hilos simultáneamente.
//  


#include "common_mutex.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Mutex::Mutex() {
	pthread_mutex_init(&this->mutex, 0);
	pthread_cond_init(&this->cond_var, 0);
}


// Constructor privado
Mutex::Mutex(const Mutex &c) { }


// Destructor
Mutex::~Mutex() {
	pthread_mutex_destroy(&this->mutex);
	pthread_cond_destroy(&this->cond_var);
}


// Bloquea la ejecución en un una condition variable hasta que se produzca
// una señalización.
void Mutex::wait() {
	pthread_cond_wait(&this->cond_var, &this->mutex);
}


// Desbloquea al menos uno de los hilos que están bloqueados en la
// condition variable.
void Mutex::signal() {
	pthread_cond_signal(&this->cond_var);
}


// Desbloquea todos los hilos bloqueados actualmente en la condition
// variable.
void Mutex::broadcast() {
	pthread_cond_broadcast(&this->cond_var);
}


// Bloquea el mutex
void Mutex::lock() {
	pthread_mutex_lock(&this->mutex);
}


// Desbloquea el mutex
void Mutex::unlock() {
	pthread_mutex_unlock(&this->mutex);
}
