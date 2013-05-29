//  
//  client_manejador_archivos.h
//  CLASE MANEJADORARCHIVOS
//  



#include "client_inspector.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
// PRE: 'intervalo' es el intervalo de inspección en segundos.
Inspector::Inspector(int intervalo) : intervalo(intervalo) { }


// Destructor
Inspector::~Inspector() { }


//
void Inspector::iniciar() {}

// 
void Inspector::establecerIntervaloDeInspeccion(int segundos) {}

//
void Inspector::forzarInspeccion() {}

// Define tareas a ejecutar en el hilo.
// Realiza una inspección cada un intervalo predeterminado.
void Inspector::run() {}

