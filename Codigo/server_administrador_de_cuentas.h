// Encargado de administrar altas, bajas y modificaciones de cuentas
// de usuario

#ifndef SERVER_ADMINISTRADOR_DE_CUENTAS_H
#define SERVER_ADMINISTRADOR_DE_CUENTAS_H

#include <iostream>
#include <fstream>
#include <string>
#include "common_lista.h"
#include "common_mutex.h"
#include "common_lock.h"
#include "server_configuracion.h"

class AdministradorDeCuentas {
private:
	// Mutex
	Mutex m;

	// Se crea el archivo si no existe con permisos especiales
	void crearArchivo(const std::string &nombre);

	// Se crea el directorio si no existe con permisos especiales
	void crearDir(const std::string &nombre);

	// Se elimina un directorio y su contenido
	void eliminarDir(const std::string &nombre);

	// Verifica si el archivo existe
	bool existeArchivo(const std::string &nombre);

	// Busqueda secuencial. Devuelve 1 si encuentra clave y usuario 
	// correctamente, 2 si encuentra usuario y no clave correcta,
	// y -1 si no lo encuentra
	int buscarCliente(std::string &usuario, std::string &clave, 
		std::string &carpeta);

	// Devuelve la lista de carpetas existentes en el directorio
	void obtenerListaCarpetas(Lista<std::string> &listaCarpetas);

public:
	// Ctor
	AdministradorDeCuentas();

	// Dtor
	~AdministradorDeCuentas();

	// Devuelve la lista de nombres de usuarios que se encuentran 
	// registrados
	void obtenerListaUsuarios(Lista<std::string> &listaUsuarios);

	// Comprueba nombre de usuario y clave de los clientes
	// POST: si la verificación es exitosa, se almacena en 'nombreUsuario' el
	// nombre de usuario del cliente.
	int verificarCliente(std::string &args, std::string& nombreUsuario, 
		std::string &pathCarpeta);

	// Agrega un cliente a la lista
	void agregarCliente(const std::string &nombre, const std::string 
		&clave);

	// Se elimina un cliente existente
	void eliminarCliente(const std::string &nombre);

	// Se modifica un cliente existente
	void modificarCliente(const std::string &nombre,
			const std::string &dato, bool esModifNombre);
};

#endif
