//
//  client_inspector.h
//  CLASE INSPECTOR
//  


#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <string>
#include "common_thread.h"
#include "client_sincronizador.h"
#include "client_manejador_de_archivos.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class Inspector : public Thread {
private:

	ManejadorDeArchivos *manejadorDeArchivos;		// Manejador de archivos
	Sincronizador *sincronizador;					// Sincronizador
	unsigned int intervalo;							// Intervalo de inspección
													// en segundos

	// Bloquea actividades hasta que haya transcurrido el intervalo de polling
	void alarmaDeInspeccion();

public:

	// Constructor
	// PRE: 'intervalo' es el intervalo de inspección en segundos.
	Inspector(ManejadorDeArchivos *unManejador, Sincronizador *sincronizador,
		unsigned int intervalo);

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

	// Se encarga de corroborar si existe o no un archivo, y en caso de existir
	// corrobora si hay diferencias entre el pasado por parámetro y el que se
	// encuentra localmente.
	// En caso de no existir el archivo, se encarga de realizar la petición del
	// mismo, mientras que si existe pero es una versión vieja, se encarga de
	// realizar la petición del bloque necesario para su actualización hacia
	// la versión mas reciente.
	// PRE: 'archivo' es un Archivo que contiene los datos del archivo a
	// comparar con el local.
	void inspeccionarArchivo(Archivo *archivo);
};

#endif
