// Seguridad que utiliza hmac_sha1

#ifndef COMMON_SEGURIDAD_H
#define COMMON_SEGURIDAD_H

#include "common_hash.h"
#include <string>
#include <cstring>

class Seguridad {
private:
	// Se realiza clave XOR cadena
	static std::string XOR(const std::string &clave, const std::string &cadena);

public:
	// Se obtiene la firma del mensaje a ser enviado con la clave pasada por parametros
	static std::string obtenerFirma(const std::string &mensaje, const std::string &clave);

	// Se compara la firma pasada por parametros con la firma que se calcula sobre el mensaje original
	// Devuelve 'true' si es valida, o 'false' sino
	static bool firmaValida(const std::string &mensaje, const std::string &clave, const std::string &firmaRecibida);
};

#endif /* COMMON_SEGURIDAD_H */	
