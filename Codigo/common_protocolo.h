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
// FORMATO DE LAS CONSTANTES: C_[instruccion]
 const std::string C_LOGIN_REQUEST = "LOGIN-REQUEST";

// Constantes para los identificadores de instrucciones enviadas por el
// servidor
// FORMATO DE LAS CONSTANTES: S_[instruccion]
const std::string S_LOGIN_OK = "LOGIN-OK";
const std::string S_LOGIN_FAIL = "LOGIN-FAIL";

// Constantes para los identificadores de instrucciones comunes al servidor
// y el cliente
// FORMATO DE LAS CONSTANTES: COMMON_[instruccion]
const std::string COMMON_SEND_FILE = "SEND-FILE";

// Constante para caracter de fin de instrucción
const char FIN_MENSAJE = '\n';


#endif
