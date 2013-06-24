//  
//  common_convertir.h
//  LIBRERIA CONVERTIR
//  
//  Librería de funciones conversoras.
//  


#include <iomanip>
#include <sstream>
#include "common_convertir.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Devuelve el equivalente entero de un caracter hexadecimal
int Convertir::htoi(char a) {
	if(a > 'F') return -1;
	else if (a < 'A') return (a - '0');
	return (a - 'A' + 10);
}


// Convierte un unsigned int a un string de contenido hexadecimal
std::string Convertir::uitoh(uint8_t *a, size_t size) {
	std::stringstream stream;

	for(unsigned int i = 0; i < size; i++) {
		stream << std::uppercase << std::setfill('0') << std::setw(2) << 
			std::hex << int(a[i]);
	}

	return stream.str();
}


// Convierte un string de contenido hexadecimal a un unsigned int
uint8_t* Convertir::htoui(const std::string& s) {
	uint8_t *a = new uint8_t[s.size() / 2];
	int j = 0;

	for(unsigned int i = 0; i < s.size(); i += 2) {
		uint8_t pri = Convertir::htoi(s[i]);
		uint8_t seg = Convertir::htoi(s[i+1]);

		a[j] = pri * 16 + seg;
		j++;
	}

	return a;
}


// Convierte un string en un integer
int Convertir::stoi(const std::string& s) {
	int i;
	std::stringstream ss(s);
	ss >> i;
	return i;
}


// Convierte un string en un unsigned integer
unsigned int Convertir::stoui(const std::string& s) {
	unsigned int i;
	std::stringstream ss(s);
	ss >> i;
	return i;
}


// Convierte un integer en un string
std::string Convertir::itos(const int i) {
    std::ostringstream s;
    s << i;
    return s.str();
}


// Convierte un unsigned integer en un string
std::string Convertir::uitos(const unsigned int i) {
	std::ostringstream s;
    s << i;
    return s.str();
}


// Convierte un string en un string en minusculas
std::string Convertir::toLowercase(const std::string &s) {
	std::string d;
	int i;
	for (i = 0; i < (int)s.length(); i++) {
		if (isalpha(s[i]))
			d += (char)tolower(s[i]);
		else
			d += s[i];
	}
	return d;
}

