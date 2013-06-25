#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <utility>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <stdint.h>



// Convierte un unsigned int a un string de contenido hexadecimal
std::string uitoh(uint8_t *a, size_t size) {
	std::stringstream stream;

	for(unsigned int i = 0; i < size; i++) {
		stream << std::uppercase << std::setfill('0') << std::setw(2) << 
			std::hex << int(a[i]);
	}

	return stream.str();
}




std::string convertir(uint8_t* mensaje, size_t longitud) {
	char* buffer = new char[longitud*2];
	std::string hexa;
	size_t i;
	// memset(buffer, 0, longitud*2);

	for (i = 0; i < longitud; i++) {
		sprintf(&buffer[i*2], "%2X", mensaje[i]);
	}

	hexa.append(buffer, longitud*2);

	delete[]buffer;

	return(hexa);
}




/* ****************************************************************************
 * PROGRAMA PRINCIPAL
 * ***************************************************************************/


int main(int argc, char* argv[]) {
	
	std::string ruta = "a10";

	// Abrimos el archivo
	std::ifstream archivo(ruta.c_str(), 
		std::ios::in | std::ios::binary | std::ios::ate);

	size_t size = archivo.tellg();
	size += 5;
	archivo.seekg(0);

	uint8_t* contenidoTemp = new uint8_t[size];

	archivo.read((char*)contenidoTemp, size);

	std::string contenidoHex(uitoh((uint8_t*)contenidoTemp, (size_t)(size)));

	std::cout << contenidoHex << std::endl;
	std::cout.flush();

	delete[] contenidoTemp;

	archivo.close();

	return 0;
}
