#include "server_verificador.h"

Verificador::Verificador() {
	this->mutex = new Mutex();
	this->logins = new std::fstream("Users_Pass.txt", std::ios_base::in |
			std::ios_base::out | std::fstream::app);
	this->logins->close();
	this->new_logins = new std::fstream("New_Users_Pass.txt", std::ios_base::in 
		| std::ios_base::out | std::fstream::app);
	this->new_logins->close();
}

Verificador::~Verificador() {
	if (this->mutex) {
		delete (this->mutex);
		this->mutex = NULL;
	}
	if (this->logins) {
		this->logins->close();
		delete(this->logins);
		this->logins = NULL;
	}
	if (this->new_logins) {
		this->new_logins->close();
		delete(this->new_logins);
		this->new_logins = NULL;
	}
}

int Verificador::agregarCliente(std::string &args) {
	std::cout << "Estoy por agregar cliente " << std::endl;
	if (this->verificarCliente(args) == 0) {
		// Agrego cliente
		std::cout << "No existe cliente y lo estoy creando" << std::endl;
		Lock lock(*this->mutex);
		// Si no esta abierto el archivo, lo abre
		if (!this->new_logins->is_open())
			this->new_logins->open("New_Users_Pass.txt", std::ios_base::in 
				| std::ios_base::out | std::fstream::app);
		// Va al final del archivo
		this->new_logins->seekg(0, std::ios_base::end);
		args.append("\n");
		this->new_logins->write(args.c_str(), args.length());
		this->new_logins->close();
		return 1;
	}
	else {
		// Cliente duplicado
		return 0;
	}
}

int Verificador::verificarCliente(std::string &args) {
	std::cout << "Estoy verificando si existe cliente" << std::endl;
	int delim = args.find('-', 0);
	std::string usuario = args.substr(0, delim);
	std::string clave = args.substr(delim + 1, std::string::npos);
	int existe = -1;
	// Busca primero en el archivo de nuevos users
	if (this->new_logins) {
		Lock lock(*this->mutex);
		// Si no esta abierto el archivo, lo abre
		if (!this->new_logins->is_open())
			this->new_logins->open("New_Users_Pass.txt", std::ios_base::in);
		existe = this->buscarClienteEn(this->new_logins, usuario, clave);
		this->new_logins->close();
	}
	if (existe == 0) {  // No lo encontre en el archivo de nuevos usuarios
		if (this->logins) {  // Lo busco en el archivo principal
			Lock lock(*this->mutex);
			// Si no esta abierto el archivo, lo abre
			if (!this->logins->is_open())
				this->logins->open("Users_Pass.txt", std::ios_base::in);
			existe = this->buscarClienteEn(this->logins, usuario, clave);
			this->logins->close();
		}
	}
	return existe;
}


// Metodos privados

int Verificador::buscarClienteEn(std::fstream* archivo,
		std::string &usuario, std::string &clave) {
	// Va al principio
	archivo->seekg(0, std::ios_base::beg);
	// Busca uno por uno si encuentra al cliente
	std::string usuarioActual;
	std::string claveActual;
	char buffer[TAM_BUF];
	std::string linea;
	int delim;
	while (!archivo->eof()) {
		for (int i = 0; i < TAM_BUF; i++) buffer[i] = '\0';
		archivo->getline(buffer, TAM_BUF);
			if (archivo->fail())
				return 0;
		linea.assign(buffer);
		delim = linea.find('-', 0);
		usuarioActual = linea.substr(0, delim);
		claveActual = linea.substr(delim + 1, std::string::npos);
		if (usuario.compare(usuarioActual) == 0) {
			if (clave.compare(claveActual) == 0)
				return 1;  // Encontre usuario y clave
			return 2;  // Encontre usuario y no clave
		}
	}
	return 0;  // No encontre ni usuario ni clave
}

void Verificador::mergeLogins() {}
