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





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
// PRE: 'intervalo' es el intervalo de inspección en segundos.
Inspector::Inspector(ManejadorDeArchivos *unManejador, 
	Sincronizador *sincronizador, unsigned int intervalo, Logger *logger) : 
	manejadorDeArchivos(unManejador), sincronizador(sincronizador), 
	intervalo(intervalo), logger(logger) { }


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

		// Bloqueamos el mutex
		Lock l(m);

		// Realizamos la inspección
		Cola< std::pair< std::string, std::string > > nuevos;
		Cola< std::pair< std::string, Lista< int > > > modificados;
		Cola< std::string > eliminados;

		if(this->manejadorDeArchivos->actualizarRegistroDeArchivos(&nuevos,
			&modificados, &eliminados)) {
			// Mensaje de log
			this->logger->emitirLog("INSPECTOR: Se detectaron cambios.");

			while(!nuevos.vacia()) {
				// Tomamos nuevo
				std::pair< std::string, std::string > nuevo;
				nuevo = nuevos.pop_bloqueante();
				std::string contenido;
				
				try {
					contenido = this->manejadorDeArchivos->obtenerContenido(
						nuevo.first);
				}
				catch(char const * e) {
					// Si no es posible abrir el archivo, dejamos que la
					// próxima inspección se encarge de detectar que debe
					// hacer con este.
					continue;
				}

				// Mensaje de log
				this->logger->emitirLog("INSPECTOR: Archivo nuevo '" + 
					nuevo.first + "'");

				// Enviamos al sincronizador
				this->sincronizador->enviarArchivo(nuevo.first, contenido,
					nuevo.second);
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

					try {
						contenido = this->manejadorDeArchivos->obtenerContenido
							(mod.first, bloque);
					}
					catch(char const * e) {
						// Si no es posible abrir el archivo, dejamos que la
						// próxima inspección se encarge de detectar que debe
						// hacer con este.
						continue;
					}

					// Insertamos par de bloques con sus contenidos en lista
					bloques.insertarUltimo(std::make_pair(bloque, contenido));
				}

				// Mensaje de log
				this->logger->emitirLog("INSPECTOR: Archivo '" + mod.first 
					+ "' fue modificado.");

				// Enviamos modificaciones del archivo al sincronizador
				this->sincronizador->modificarArchivo(mod.first, 
					this->manejadorDeArchivos->obtenerCantBytes(mod.first),
						bloques);
			}

			while(!eliminados.vacia()) {
				// Tomamos eliminado
				std::string elim = eliminados.pop_bloqueante();

				// Mensaje de log
				this->logger->emitirLog("INSPECTOR: Archivo '" + elim 
					+ "' fue eliminado.");

				// Enviamos a sincronizador
				this->sincronizador->eliminarArchivo(elim);
			}
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
	// Bloqueamos el mutex
	Lock l(m);

	// Mensaje de log
	this->logger->emitirLog("INSPECTOR: Inspeccionando archivo '" +
		nombreArchivo + "' en directorio local.");

	// Si no existe, lo solicitamos al servidor
	if(!this->manejadorDeArchivos->existeArchivo(nombreArchivo)) {
		this->sincronizador->solicitarArchivoNuevo(nombreArchivo);
		return;
	}

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
		solicitar = !this->manejadorDeArchivos->compararBloque(nombreArchivo,
			bloques.verPrimero().first, bloques.verPrimero().second);

		// Si se requiere el1 bloque, lo listamos para solicitarlo
		if(solicitar) 
			bloquesASolicitar.insertarUltimo(bloques.verPrimero().first);
		
		bloques.eliminarPrimero();
	}
	
	// Si los bloques son compatibles con la versión enviada, retornamos
	if(bloquesASolicitar.estaVacia()) return;

	// Solicitamos al servidor los bloques
	this->sincronizador->solicitarBloquesModificados(nombreArchivo,
		bloquesASolicitar);
}


// Inspecciona si existe un archivo en el directorio local. Si no existe
// se encarga de indicar que debe ser solicitado al servidor.
void Inspector::inspeccionarExisteArchivo(std::string& nombreArchivo,
	std::string hashArchivo) {
	// Bloqueamos el mutex
	Lock l(m);

	// Mensaje de log
	this->logger->emitirLog("INSPECTOR: Inspeccionando existencia de archivo '"
		+ nombreArchivo + "' en directorio local.");

	// Corroboramos si existe el archivo
	if(this->manejadorDeArchivos->existeArchivoEnRegitro(nombreArchivo)) {
		// Tomamos el hash del archivo local
		std::string hashArchivoLoc;
		this->manejadorDeArchivos->obtenerHash(nombreArchivo, hashArchivoLoc);

		// Si tenemos archivos iguales, retornamos sin solicitar nada
		if(hashArchivo == hashArchivoLoc) 
			return;
		// Sino, se debe pedir
		else {
			this->sincronizador->solicitarArchivoNuevo(nombreArchivo);
			return;
		}
	}

	// Si no existe o es viejo, lo solicitamos al servidor
	if(!this->manejadorDeArchivos->existeArchivo(nombreArchivo))
		this->sincronizador->solicitarArchivoNuevo(nombreArchivo);
}





/*
 * IMPLEMENTACIÓN DE MÉTODOS PRIVADOS DE LA CLASE
 */


// Bloquea actividades hasta que haya transcurrido el intervalo de polling
void Inspector::alarmaDeInspeccion() {
	this->sleep(this->intervalo);
}
