#include "server_verificador.h"

Verificador::Verificador() {
	this->mutex = new Mutex();
	this->logins = new std::fstream("Users_Pass.txt", std::ios_base::in);
}

Verificador::~Verificador() {
	if (this->mutex) {
		delete (this->mutex);
		this->mutex = NULL;
	}
	if (this->logins) {
		if (this->logins->is_open())
			this->logins->close();
		delete(this->logins);
		this->logins = NULL;
	}
}

int Verificador::verificarCliente(std::string &args) {
	std::cout << "Estoy verificando si existe cliente" << std::endl;
	int delim = args.find('-', 0);
	std::string usuario = args.substr(0, delim);
	std::string clave = args.substr(delim + 1, std::string::npos);
	int existe = -1;
	if (this->logins) {  // Lo busco en el archivo de usuarios y claves
		Lock lock(*this->mutex);
		// Si no esta abierto el archivo, lo abre
		if (!this->logins->is_open())
			this->logins->open("Users_Pass.txt", std::ios_base::in);
		existe = this->buscarCliente(usuario, clave);
	}
	return existe;
}


// Metodos privados

int Verificador::buscarCliente(std::string &usuario, std::string &clave) {
	// Va al principio
	this->logins->seekg(0, std::ios_base::beg);
	// Busca uno por uno si encuentra al cliente
	std::string usuarioActual;
	std::string claveActual;
	char buffer[TAM_BUF];
	std::string linea;
	int delim;
	while (!this->logins->eof()) {
		for (int i = 0; i < TAM_BUF; i++) buffer[i] = '\0';
		this->logins->getline(buffer, TAM_BUF);
			if (this->logins->fail())
				return 0;
		linea.assign(buffer);
		delim = linea.find('-', 0);
		usuarioActual.assign(buffer, delim);
		claveActual = linea.substr(delim + 1, std::string::npos);
		if (usuario.compare(usuarioActual) == 0) {
			if (clave.compare(claveActual) == 0)
				return 1;  // Encontre usuario y clave
			return 2;  // Encontre usuario y no clave
		}
	}
	return 0;  // No encontre ni usuario ni clave
}

