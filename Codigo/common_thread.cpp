//  
//  common_thread.cpp
//  CLASE THREAD
//  
//  Clase que implementa la interfaz para la creación de un hilo de ejecución.
//  


#include "common_thread.h"
#include <signal.h>
#include <time.h>




/* ****************************************************************************
 * Funciones auxiliares
 * ***************************************************************************/

// Función auxiliar que representa el manejador de la alarma, utilizado para la
// interrupción de la suspensión de un hilo.
void alarm_handler(int signal) { }





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
Thread::Thread() : status(false) { }


// Constructor privado
Thread::Thread(const Thread &c) { }


// Destructor
Thread::~Thread() { }


// Inicia el hilo
void Thread::start() {
	pthread_create(&this->thread, 0, callback, this);
}


// Detiene el hilo
void Thread::stop() {
	this->status = false;
}


// Envía una solicitud de cancelación al hilo, deteniendo abruptamente
// su ejecución
void Thread::cancel() {
	pthread_cancel(this->thread);
}


// Bloquea hasta que el hilo finalice su ejecución en caso de estar
// ejecutandose.
void Thread::join() {
	pthread_join(this->thread, 0);
}


// Suspende la ejecución del hilo durante cierto intervalo de tiempo.
// Puede ser interrumpido llamando al metodo kill().
void Thread::sleep(unsigned int seconds) {
	struct timespec t, t_aux;
	t.tv_sec = seconds;
	t.tv_nsec = 0;
	nanosleep(&t, &t_aux);

	// Registramos un manejador de alarma para poder interrumpur la suspensión
	signal(SIGALRM, alarm_handler);
}


// Envía una señal  al hilo.
void Thread::kill() {
	pthread_kill(this->thread, SIGALRM);
}


// Verifica si el hilo se encuentra activo.
// POST: devuelve true si está activo o false en caso contrario.
bool Thread::isActive() {
	return this->status;
}


// Ejecuta el método run().
// PRE: 'threadID' es un puntero al thread.
void* Thread::callback(void *threadID) {
	((Thread*)threadID)->status = true;
	((Thread*)threadID)->run();
	((Thread*)threadID)->status = false;
	return 0;
}
