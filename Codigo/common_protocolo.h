//  
//  common_protocolo.h
//  
//  Cabecera con constantes que especifican el protocolo de mensajes a
//  ser utilizados por el cliente y el servidor.
//  


#ifndef PROTOCOLO_H
#define PROTOCOLO_H




/* ****************************************************************************
 * PROTOCOLO DE INSTRUCCIONES
 * ***************************************************************************/


// Constantes para los identificadores de instrucciones enviadas por el
// cliente
const std::string C_GET_JOB_PART = "GET-JOB-PART";
const std::string C_POSSIBLE_KEY = "POSSIBLE-KEY";
const std::string C_JOB_PART_FINISHED = "JOB-PART-FINISHED";

// Constantes para los identificadores de instrucciones enviadas por el
// servidor
const std::string S_JOB_PART = "JOB-PART";
const std::string S_NO_JOB_PART = "NO-JOB-PART";

// Constante para caracter de fin de instrucción
const char FIN_MENSAJE = '\n';


#endif
