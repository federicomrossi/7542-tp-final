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

				// Lista de bloques auxiliar
				Lista< std::pair< int, std::string > > bloques;

				// Iteramos sobre los bloques que fueron modificados
				while(!mod.second.estaVacia()) {
					// Tomamos el número de bloque y su contenido
					int bloque = mod.second.verPrimero();
					mod.second.eliminarPrimero();
					std::string contenido;
					contenido = this->manejadorDeArchivos->obtenerContenido(
						mod.first, bloque);

					// Insertamos par de bloques con sus contenidos en lista
					bloques.insertarUltimo(std::make_pair(bloque, contenido));
				}

				// Enviamos modificaciones del archivo al sincronizador
				this->sincronizador->modificarArchivo(mod.first, 
					this->manejadorDeArchivos->obtenerCantBytes(mod.first),
						bloques);

				// DEBUG
				std::cout << "Modificado: " << mod.first << std::endl;
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


// Inspecciona si los hashes de los bloques pasados por parámetro
// coinciden con los del archivo local. Si no coinciden, se encarga de
// indicar que bloques deben ser solicitados al servidor.
// PRE: 'nombreArchivo' es el nombre del archivo a verificar;
// 'cantBYtesTotal' es la cantidad total de bytes que debe tener el
// archivo; bloques es una lista de pares (bloque, hash), con los números
// de bloque a verificar.
void Inspector::inspeccionarArchivo(std::string nombreArchivo, unsigned int&
		cantBytesTotal, Lista< std::pair< int, std::string > > bloques) {
	// Tomamos bytes actuales de archivo local
	unsigned int b;
	b = this->manejadorDeArchivos->obtenerCantBytes(nombreArchivo);

	// Caso en que solo se modificó el largo del archivo
	if(bloques.estaVacia() && (cantBytesTotal != b)) {
		Lista< std::pair< int, std::string > > auxiliar;

		// Enviamos a modificar el archivo para que se achice su tamaño
		this->manejadorDeArchivos->modificarArchivo(nombreArchivo, 
			cantBytesTotal, auxiliar);

		return;
	}

	Lista< int > bloquesASolicitar;

	// Corroboramos que bloques se necesitan medir
	while(!bloques.estaVacia()) {
		// Comprobamos si es necesario solicitar el bloque
		bool solicitar;
		solicitar = this->manejadorDeArchivos->compararBloque(nombreArchivo,
			bloques.verPrimero().first, bloques.verPrimero().second);

		// Si se requiere el bloque, lo listamos para solicitarlo
		if(solicitar) 
			bloquesASolicitar.insertarUltimo(bloques.verPrimero().first);
	}

	// Si los bloques son compatibles con la versión enviada, retornamos
	if(bloquesASolicitar.estaVacia()) return;

	// Solicitamos al servidor los bloques
	this->sincronizador->solicitarBloquesModificados(nombreArchivo,
		bloquesASolicitar);
}


// Inspecciona si existe un archivo en el directorio local. Si no existe
// se encarga de indicar que debe ser solicitado al servidor.
void Inspector::inspeccionarExisteArchivo(std::string& nombreArchivo) {
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
