#include "common_seguridad.h"


// Se obtiene la firma del mensaje a ser enviado con la clave pasada por parametros
std::string Seguridad::obtenerFirma(const std::string &mensaje, const std::string &clave) {
	char ipad[64];  // inner
	char opad[64];  // outer
	std::string k = clave;
	
	// Se inicializan las variables anteriores en 0x36 y 0x5C
	memset(ipad, 0x36, 64);
	memset(opad, 0x5C, 64);

	if (k.length() < 64)
		// Se hace padding con 0 en la clave hasta completar 64 bytes
		k.append(64 - clave.length(), 0x00);
	else {
		// Se calcula el hash de la clave para acortarla
		k = Hash::funcionDeHashBin(k);
	}

	// K XOR ipad -> primerHash
	std::string primerHash = XOR(k, ipad);

	// K XOR opad ->segundoHash
	std::string segundoHash = XOR(k, opad);

	// Se agrega el mensaje
	primerHash += mensaje;

	// Hash (primerHash) -> primerHash
	primerHash = Hash::funcionDeHash(primerHash);

	// Se unen segundoHash + primerHash
	segundoHash.append(primerHash);	

	// Hash (segundoHash + primerHash) -> segundoHash
	segundoHash = Hash::funcionDeHash(segundoHash);

	return segundoHash;
}

// Se compara la firma pasada por parametros con la firma que se calcula sobre el mensaje original
bool Seguridad::firmaValida(const std::string &mensaje, const std::string &clave, const std::string &firmaRecibida) {
	if (obtenerFirma(mensaje, clave) == firmaRecibida)
		return true;
	return false;
}


/* Implementacion de metodos privados */

// Devuelve clave XOR cadena
std::string Seguridad::XOR(const std::string &clave, const std::string &cadena) {
	int i;
	std::string aux;
	for (i = 0; i < 64; i++)
		aux += (clave[i] ^ cadena[i]);
	return aux;
}

