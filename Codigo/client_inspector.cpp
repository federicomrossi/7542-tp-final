//  
//  client_inspector.h
//  CLASE INSPECTOR
//  



#include "client_inspector.h"
#include "common_cola.h"
#include "common_lista.h"
#include <string>
#include <utility>
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


// Define tareas a ejecutar en el hilo.
// Realiza una inspección cada un intervalo predeterminado.
void Inspector::run() {
	// Inspeccionamos cada cierto intervalo hasta detener hilo
	while(this->isActive()) {
		// Nos detenemos hasta que suene la alarma de inspección
		this->alarmaDeInspeccion();
		
		// Si se detuvo al inspector, salimos
		if(!this->isActive()) return;

		// // DEBUG
		// std::string hash_aux;
		// this->manejadorDeArchivos->obtenerHash("aa", hash_aux);
		// std::cout << "Hash: " << hash_aux << std::endl;
		// continue;
		// // END DEBUG

		// Realizamos la inspección
		Cola< std::string > nuevos;
		Cola< std::pair< std::string, Lista< int > > > modificados;
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
				std::string contenido;
				contenido = this->manejadorDeArchivos->obtenerContenido(nuevo);

				// Enviamos al sincronizador
				this->sincronizador->enviarArchivo(nuevo, contenido);
				
				// DEBUG
				std::cout << "Nuevo: " << nuevo << std::endl;
				// END DEBUG
			}

			while(!modificados.vacia()) {
				// Tomamos modificado
				std::pair< std::string, Lista<int> > mod;
				mod = modificados.pop_bloqueante();


				// Lista< std::pair< int, std::string > > bloques;

				// while(!mod.estaVacia()) {
					
				// }
				// 	std::cout << mod.second[i];

				// // Enviamos a sincronizador
				// this->sincronizador->modificarArchivo(mod.first, mod.second);

				// DEBUG
				std::cout << "Modificado: " << mod.first << std::endl;

				std::cout << "Bloques: ";

				for(size_t i = 0; i < mod.second.tamanio(); i++)
					std::cout << mod.second[i];

				std::cout << std::endl;
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


// Se encarga de corroborar si existe o no un archivo, y en caso de existir
// corrobora si hay diferencias entre el pasado por parámetro y el que se
// encuentra localmente.
// En caso de no existir el archivo, se encarga de realizar la petición del
// mismo, mientras que si existe pero es una versión vieja, se encarga de
// realizar la petición del bloque necesario para su actualización hacia
// la versión mas reciente.
// PRE: 'archivo' es un Archivo que contiene los datos del archivo a
// comparar con el local.
void Inspector::inspeccionarArchivo(Archivo *archivo) {
	// Objetos auxiliares
	// Archivo archivoTemp;

	// // Corroboramos si existe el archivo
	// if(!this->manejadorDeArchivos->obtenerArchivo(archivo->obtenerNombre(),
	// 	archivoTemp)) {
	// 	// Si no existe, lo solicitamos al servidor
	// 	std::string a = archivo->obtenerNombre();
	// 	this->sincronizador->solicitarArchivoNuevo(a);
	// 	return;
	// }

	// Como existe, corroboramos si el local es el mas reciente, es decir,
	// igual al pasado por parámetro

	// [ INSERTAR CODIGO ]
}



//
void Inspector::inspeccionarExisteArchivo(std::string nombreArchivo) {
	// Corroboramos si existe el archivo
	if(this->manejadorDeArchivos->existeArchivoEnRegitro(nombreArchivo))
		return;

	// Si no existe, lo solicitamos al servidor
	this->sincronizador->solicitarArchivoNuevo(nombreArchivo);
}





/*
 * IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */


// Bloquea actividades hasta que haya transcurrido el intervalo de polling
void Inspector::alarmaDeInspeccion() {
	this->sleep(this->intervalo);
}
