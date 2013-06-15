#include "server_verificador.h"

Verificador::Verificador() {
	this->mutex = new Mutex();
	this->logins = new std::fstream("servidor/Users_Pass.txt", std::ios_base::in);
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
	std::string& nombreUsuario) {
	// DEBUG
	std::cout << "Estoy verificando si existe cliente" << std::endl;
	//END DEBUG
	
	// Se obtiene usuario y clave
	int delim = args.find(COMMON_DELIMITER, 0);
	std::string usuario = args.substr(0, delim);
	std::string clave = args.substr(delim + 1, std::string::npos);
	int existe = -1;
	
	// Lo busco en el archivo de usuarios y claves
	if (this->logins) {  
		Lock lock(*this->mutex);
		// Si no esta abierto el archivo, lo abre
		if (!this->logins->is_open())
			this->logins->open("servidor/Users_Pass.txt", std::ios_base::in);
		existe = this->buscarCliente(usuario, clave);
		// Cierra el archivo
		this->logins->close();
	}

	// Si la validación es exitosa, almacenamos el nombre de usuario
	if(existe) nombreUsuario = usuario;

	return existe;
}


// Metodos privados

int Verificador::buscarCliente(std::string &usuario, std::string &clave) {

	std::string usuarioActual;
	std::string claveActual;
	char buffer[TAM_BUF];
	std::string linea;
	int delim, codigo = -1;

	// Va al principio del archivo
	this->logins->seekg(0, std::ios_base::beg);

	// Busca uno por uno si encuentra al cliente
	while (!this->logins->eof()) {
		memset((void*)buffer, '\0', TAM_BUF);
		// Lee una linea
		this->logins->getline(buffer, TAM_BUF);
		if (!this->logins->fail()) {
			linea.assign(buffer);
			delim = linea.find(COMMON_DELIMITER, 0);
			usuarioActual.assign(buffer, delim);
			claveActual = linea.substr(delim + 1, std::string::npos);
			// Compara
			if (usuario.compare(usuarioActual) == 0) {
				if (clave.compare(claveActual) == 0) {
					codigo = 1;  // Encontre usuario y clave
					break;
				}
				codigo = 2;  // Encontre usuario y no clave
				break;
			}
		}
		else
			break;
	}
	return codigo;  // No encontre ni usuario ni clave
}

