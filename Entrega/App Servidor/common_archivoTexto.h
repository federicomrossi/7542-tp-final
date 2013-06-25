//
//  common_archivoTexto.h
//  CLASE ARCHIVOTEXTO
//  


#ifndef __ARCHIVOTEXTO_H__
#define __ARCHIVOTEXTO_H__

#ifndef MAX_LENGTH
#define MAX_LENGTH 25
#endif

#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

class ArchivoTexto {
private:
	fstream  archivo;  // referencia al archivo

public:
	ArchivoTexto(const std::string& path,int flag);
	~ArchivoTexto();
	void escribir(const std::string& cadena);
	bool leerLinea(std::string &cadena, char separador,string buscado);
	void leerLinea(string& cadena, int pos); 
	bool validarLinea(std:: string &cadena, string buscado);
};

#endif
