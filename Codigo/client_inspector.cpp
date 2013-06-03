//  
//  client_inspector.h
//  CLASE INSPECTOR
//  



#include "client_inspector.h"
#include "client_manejador_de_archivos.h"
#include "common_cola.h"
#include <unistd.h>

// DEBUG
#include <iostream> // REMOVER LUEGO DE SACAR STDCOUT
// END DEBUG



/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
// PRE: 'intervalo' es el intervalo de inspección en segundos.
Inspector::Inspector(unsigned int intervalo) : intervalo(intervalo) { }


// Destructor
Inspector::~Inspector() { }


// Inicia el ciclo de inspecciones
void Inspector::iniciar() {
	this->start();
}


// Detiene el ciclo de inspecciones
void Inspector::detener() {
	this->stop();

	// Enviamos señal para detener el intervalo de inspección
	this->kill();
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

		ManejadorDeArchivos ma("cliente");
		if(ma.actualizarRegistroDeArchivos(&nuevos, &modificados, &eliminados))
		{
			// DEBUG
			std::cout << "Inspeccion: hubieron cambios" << std::endl;
			std::cout.flush();

			while(!nuevos.vacia())
				std::cout << "Nuevo: " << nuevos.pop_bloqueante() << std::endl;

			while(!modificados.vacia())
				std::cout << "Modificado: " << modificados.pop_bloqueante() << std::endl;

			while(!eliminados.vacia())
				std::cout << "Eliminado: " << eliminados.pop_bloqueante() << std::endl;

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

