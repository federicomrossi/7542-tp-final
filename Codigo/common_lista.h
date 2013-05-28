//  
//  common_lista.h
//  CLASE LISTA
//  
//  Clase que implementa una lista con la caracteristica de ser thread-safe.
//  


#ifndef LISTA_H
#define LISTA_H


#include <list>
#include "common_mutex.h"
#include "common_lock.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


template < typename Tipo > 
class Lista {
private:
	
	std::list< Tipo > lista;			// Lista
	Mutex m;							// Mutex

public:

	// Constructor
	Lista();

	// Destructor
	~Lista();

	// Inserta un nuevo elemento al final de la lista.
	// PRE: 'dato' es el dato a insertar.
	void insertarUltimo(Tipo dato);

	// Devuelve un puntero al primer elemento
	Tipo verPrimero();

	// Elimina el primer elemento de la lista.  
	// POST: se destruyó el elemento removido.
	void eliminarPrimero();

	// Elimina de la lista todos los elementos iguales al valor especificado.
	// POST: se llama al destructor de estos elementos.
	void eliminar(Tipo valor);

	// Devuelve la cantidad de elementos contenidos en la lista.
	size_t tamanio();

	// Verifica si una lista se encuentra vacía.
	// POST: Devuelve verdadero si la lista se encuentra vacía o falso en 
	// caso contrario.
	bool estaVacia();
};




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
template <typename Tipo >
Lista< Tipo >::Lista() { }


// Destructor
template <typename Tipo >
Lista< Tipo >::~Lista() { }


// Inserta un nuevo elemento al final de la lista.
// PRE: 'dato' es el dato a insertar.
template <typename Tipo >
void Lista< Tipo >::insertarUltimo(Tipo dato) {
	Lock l(m);
	this->lista.push_back(dato);
}


// Devuelve un puntero al primer elemento
template <typename Tipo >
Tipo Lista< Tipo >::verPrimero() {
	Lock l(m);
	return this->lista.front();
}


// Elimina el primer elemento de la lista.  
// POST: se destruyó el elemento removido.
template <typename Tipo >
void Lista< Tipo >::eliminarPrimero() {
	Lock l(m);
	this->lista.pop_front();
}


// Elimina de la lista todos los elementos iguales al valor especificado.
// POST: se llama al destructor de estos elementos.
template <typename Tipo >
void Lista< Tipo >::eliminar(Tipo valor) {
	Lock l(m);
	this->lista.remove(valor);
}


// Devuelve la cantidad de elementos contenidos en la lista.
template <typename Tipo >
size_t Lista< Tipo >::tamanio() {
	Lock l(m);
	return this->lista.size();
}


// Verifica si una lista se encuentra vacía.
// POST: Devuelve verdadero si la lista se encuentra vacía o falso en 
// caso contrario.
template <typename Tipo >
bool Lista< Tipo >::estaVacia() {
	Lock l(m);
	return this->lista.empty();
}


#endif
