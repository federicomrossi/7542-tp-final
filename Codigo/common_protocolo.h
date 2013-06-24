//  
//  common_protocolo.h
//  
//  Cabecera con constantes que especifican el protocolo de mensajes a
//  ser utilizados por el cliente y el servidor.
//  


#ifndef PROTOCOLO_H
#define PROTOCOLO_H

#include <string>




/* ****************************************************************************
 * PROTOCOLO DE INSTRUCCIONES
 * ***************************************************************************/


// Constantes para los identificadores de instrucciones enviadas por el
// cliente
// FORMATO DE LAS CONSTANTES: C_[instruccion]
const std::string C_LOGIN_REQUEST = "LOGIN-REQUEST";
const std::string C_GET_FILES_LIST = "GET-FILES-LIST";
const std::string C_FILE_REQUEST = "FILE-REQUEST";
const std::string C_FILE_PARTS_REQUEST = "FILE-PARTS-REQUEST";
const std::string C_MODIFY_FILE = "MODIFY-FILE";

// Constantes para los identificadores de instrucciones enviadas por el
// servidor
// FORMATO DE LAS CONSTANTES: S_[instruccion]
const std::string S_LOGIN_OK = "LOGIN-OK";
const std::string S_LOGIN_FAIL = "LOGIN-FAIL";
const std::string S_FILES_LIST = "FILES-LIST";
const std::string S_FILE_CHANGED = "FILE-CHANGED";
const std::string S_NEW_FILE = "NEW-FILE";
const std::string S_NO_SUCH_FILE = "NO-SUCH-FILE";
const std::string S_CORRUPT_MESSAGE = "CORRUPT-MESSAGE";
const std::string S_SERVER_INFO = "SERVER-INFO";
const std::string S_SERVER_USER_LIST = "SERVER-USER-LIST";
const std::string S_SERVER_USER_PASS = "SERVER-USER-PASS";


// Constates para los identificadores de instrucciones enviadas por el monitor
const std::string M_SERVER_INFO_REQUEST = "SERVER-INFO-REQUEST";
const std::string M_SERVER_USER_LIST_REQUEST = "SERVER-USER-LIST-REQUEST";
const std::string M_SERVER_NEW_USER_INFO = "SERVER-NEW-USER-INFO";
const std::string M_SERVER_DELETE_USER = "SERVER-DELETE-USER";
const std::string M_SERVER_MODIFY_USER_REQUEST = "SERVER-MODIFY-USER-REQUEST";
const std::string M_SERVER_MODIFY_USER = "SERVER-MODIFIED-USER";
const std::string M_SERVER_GET_LOG = "SERVER-GET-LOG";

// Constantes para los identificadores de instrucciones comunes al servidor
// y al cliente
// FORMATO DE LAS CONSTANTES: COMMON_[instruccion]
const std::string COMMON_SEND_FILE = "SEND-FILE";
const std::string COMMON_DELETE_FILE = "DELETE-FILE";
const std::string COMMON_FILE_PARTS = "FILE-PARTS";
const std::string COMMON_DELIMITER = "/";

// Constante para caracter de fin de instrucción
const char FIN_MENSAJE = '\n';


#endif
