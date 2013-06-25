

#include "common_archivoTexto.h"

using namespace std;

ArchivoTexto::ArchivoTexto(const std::string& path, int flag) {
	if (flag == 0) {
		archivo.open(path.c_str(), fstream::in);
		if (!archivo.is_open())
			throw ios_base::failure("No fue posible acceder al archivo de configuracion");
	}
	if (flag == 1) {
		archivo.open(path.c_str(), fstream::out | fstream::trunc);
		if (!archivo.is_open())
		throw ios_base::failure("No fue posible acceder al archivo de configuracion");
	}
}



ArchivoTexto::~ArchivoTexto() {
  archivo.close();
}


void ArchivoTexto::escribir(const string& cadena) {
	archivo << cadena;
}

void ArchivoTexto::leerLinea(string& cadena, int pos) {
	char linea[MAX_LENGTH];
	archivo.seekg(pos, archivo.beg);
	archivo.getline((char*)&linea, MAX_LENGTH,'\n');
	cadena = linea;

}

bool ArchivoTexto::validarLinea(string &cadena, string buscado) {
	int i = 0;
	int j = (int) buscado.size();
	for (i = 0; i < j ; i++) {
		if ((cadena[i]) != (buscado[i])) return false;
	}
	return true;
}

bool ArchivoTexto::leerLinea(string &cadena, char separador,string buscado) {
  char linea[MAX_LENGTH];
   //  lee del archivo a la linea, hasta haber leido:
  //  MAX_LENGTH caracteres, o un fin de linea
	  archivo.getline((char*)&linea , MAX_LENGTH, separador);
	  cadena = linea; //cadena tiene lo leido.

	  if (validarLinea(cadena,buscado)) return true;

	  return false;
}


