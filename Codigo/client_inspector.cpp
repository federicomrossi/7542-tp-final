//  
//  client_inspector.h
//  CLASE INSPECTOR
//  



#include "client_inspector.h"
#include "common_cola.h"
#include <string>
#include <unistd.h>

// DEBUG
#include <iostream> // REMOVER LUEGO DE SACAR STDCOUT
// END DEBUG




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
// PRE: 'intervalo' es el intervalo de inspección en segundos.
Inspector::Inspector(ManejadorDeArchivos *unManejador, 
	Sincronizador *sincronizador, unsigned int intervalo) : 
	manejadorDeArchivos(unManejador), sincronizador(sincronizador), 
	intervalo(intervalo) { }


// Destructor
Inspector::~Inspector() { }


// Inicia el ciclo de inspecciones
void Inspector::iniciar() {
	this->start();
}


// Detiene el ciclo de inspecciones
void Inspector::detener() {
	this->stop();

	// Interrumpimos el intervalo de inspección
	this->interruptSleep();
}


// Setea los segundos de intervalo entre sucesivas inspecciones.
// PRE: 'segundos' es la cantidad de segundos entre inspecciones.
void Inspector::establecerIntervaloDeInspeccion(unsigned int segundos) {
	this->intervalo = segundos;
}


// CONSIDERAR REMOVER ESTE METODO
void Inspector::forzarInspeccion() {

}


// Define tareas a ejecutar en el hilo.
// Realiza una inspección cada un intervalo predeterminado.
void Inspector::run() {
	// Inspeccionamos cada cierto intervalo hasta detener hilo
	while(this->isActive()) {
		// Nos detenemos hasta que suene la alarma de inspección
		this->alarmaDeInspeccion();
		
		// Si se detuvo al inspector, salimos
		if(!this->isActive()) return;

		
		// Realizamos la inspección
		Cola< std::string > nuevos;
		Cola< std::string > modificados;
		Cola< std::string > eliminados;

		if(this->manejadorDeArchivos->actualizarRegistroDeArchivos(&nuevos,
			&modificados, &eliminados))
		{
			// DEBUG
			std::cout << "Inspeccion: hubieron cambios" << std::endl;
			std::cout.flush();

			while(!nuevos.vacia()) {
				// Tomamos nuevo
				std::string nuevo = nuevos.pop_bloqueante();

				// Enviamos al sincronizador
				this->sincronizador->enviarArchivo(nuevo, 
					this->manejadorDeArchivos->obtenerContenidoArchivo(nuevo));
				
				// DEBUG
				std::cout << "Nuevo: " << nuevo << std::endl;
				// END DEBUG
			}

			while(!modificados.vacia()) {
				// Tomamos modificado
				std::string mod = modificados.pop_bloqueante();

				// Enviamos a sincronizador
				this->sincronizador->modificarArchivo(mod, 0, 
					this->manejadorDeArchivos->obtenerContenidoArchivo(mod));

				// DEBUG
				std::cout << "Modificado: " << mod << std::endl;
				// END DEBUG
			}

			while(!eliminados.vacia()) {
				// Tomamos eliminado
				std::string elim = eliminados.pop_bloqueante();

				// Enviamos a sincronizador
				this->sincronizador->eliminarArchivo(elim);
				
				// DEBUG
				std::cout << "Eliminado: " << elim << std::endl;
				// END DEBUG
			}

			// END DEBUG
		}
		else {
			// DEBUG
			std::cout << "Inspeccion: no hubieron cambios" << std::endl;
			std::cout.flush();
			// END DEBUG
		}
	}
}




/*
 * IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */


// Bloquea actividades hasta que haya transcurrido el intervalo de polling
void Inspector::alarmaDeInspeccion() {
	this->sleep(this->intervalo);
}

