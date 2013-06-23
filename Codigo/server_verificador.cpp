#include "server_verificador.h"

Verificador::Verificador() {
	this->mutex = new Mutex();
	this->logins = new std::ifstream("bd/Users_Pass.txt", 
		std::ios_base::in);
	this->logins->close();
}

Verificador::~Verificador() {
	if (this->mutex) {
		delete (this->mutex);
	}
	if (this->logins) {
		if (this->logins->is_open())
			this->logins->close();
		delete(this->logins);
	}
}

int Verificador::verificarCliente(std::string &args, 
	std::string& nombreUsuario, std::string &pathCarpeta) {
	// Variables auxiliares
	std::string usuario, clave;
	int delim;
	
	delim = args.find(COMMON_DELIMITER, 0);
	usuario = args.substr(0, delim);
	clave = args.substr(delim + 1, std::string::npos);
	int existe = -1;
	
	// Lo busco en el archivo de usuarios y claves
	if (this->logins) {  
		Lock lock(*this->mutex);
		// Si no esta abierto el archivo, lo abre
		if (!this->logins->is_open())
			this->logins->open("bd/Users_Pass.txt", std::ios_base::in);
		existe = this->buscarCliente(usuario, clave, pathCarpeta);
		// Cierra el archivo
		this->logins->close();
	}

	// Si la validación es exitosa, almacenamos el nombre de usuario
	if(existe) nombreUsuario = usuario;

	return existe;
}


// Metodos privados

int Verificador::buscarCliente(std::string &usuario, std::string &clave, 
	std::string &carpeta) {
	// Variables auxiliares
	std::string usuarioActual, claveActual, lineaActual;
	int codigo = -1;
	Lista<std::string> param;

	// Va al principio del archivo
	this->logins->seekg(0, std::ios_base::beg);

	// Busca uno por uno si encuentra al cliente
	while (getline(*logins, lineaActual)) {
		// Se parsea la linea
		Parser::dividirCadena(lineaActual, &param, COMMON_DELIMITER[0]);
		// Se obtienen los datos
		usuarioActual = param[0];
		claveActual = param[1];
		// Compara
		if (usuario.compare(usuarioActual) == 0) {
			if (clave.compare(claveActual) == 0) {
				if (param.tamanio() > 2)
					carpeta = param[2];
				codigo = 1;  // Encontre usuario y clave
				break;
			}
			codigo = 2;  // Encontre usuario y no clave
			break;
		}
		param.vaciar();
	}
	return codigo;  // No encontre ni usuario ni clave
}

