//
//  client_inspector.h
//  CLASE INSPECTOR
//  


#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <string>
#include "common_thread.h"
#include "common_lista.h"
#include "common_manejador_de_archivos.h"
#include "common_logger.h"
#include "client_sincronizador.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Inspector : public Thread {
private:

	ManejadorDeArchivos *manejadorDeArchivos;		// Manejador de archivos
	Sincronizador *sincronizador;					// Sincronizador
	unsigned int intervalo;							// Intervalo de inspección
													// en segundos
	Logger *logger;									// Logger de eventos
	Mutex m;										// Mutex

	// Bloquea actividades hasta que haya transcurrido el intervalo de polling
	void alarmaDeInspeccion();

public:

	// Constructor
	// PRE: 'intervalo' es el intervalo de inspección en segundos.
	Inspector(ManejadorDeArchivos *unManejador, Sincronizador *sincronizador,
		unsigned int intervalo, Logger *logger);

	// Destructor
	~Inspector();

	// Inicia el ciclo de inspecciones
	void iniciar();

	// Detiene el ciclo de inspecciones
	void detener();

	// Setea los segundos de intervalo entre sucesivas inspecciones.
	// PRE: 'segundos' es la cantidad de segundos entre inspecciones.
	void establecerIntervaloDeInspeccion(unsigned int segundos);

	// Define tareas a ejecutar en el hilo.
	// Realiza una inspección cada un intervalo predeterminado.
	virtual void run();

	// Inspecciona si los hashes de los bloques pasados por parámetro
	// coinciden con los del archivo local. Si no coinciden, se encarga de
	// indicar que bloques deben ser solicitados al servidor.
	// PRE: 'nombreArchivo' es el nombre del archivo a verificar;
	// 'cantBYtesTotal' es la cantidad total de bytes que debe tener el
	// archivo; bloques es una lista de pares (bloque, hash), con los números
	// de bloque a verificar.
	void inspeccionarArchivo(std::string nombreArchivo, unsigned int& 
		cantBytesTotal, Lista< std::pair< int, std::string > > bloques);

	// Inspecciona si existe un archivo en el directorio local. Si no existe
	// se encarga de indicar que debe ser solicitado al servidor.
	void inspeccionarExisteArchivo(std::string& nombreArchivo, 
		std::string hashArchivo);
};

#endif
