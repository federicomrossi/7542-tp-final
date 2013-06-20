//  
//  common_lista.h
//  CLASE LISTA
//  
//  Clase que implementa una lista con la caracteristica de ser thread-safe.
//  


#ifndef LISTA_H
#define LISTA_H


#include <list>
#include <algorithm>
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

	// Constructor copia
	Lista(const Lista< Tipo>& l);

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

	// Ordena los elementos de la lista (deben poder ser comparables)
	void ordenar();

	// Busca un elemento en la lista.
	// PRE: 'valor' es el valor del elemento a buscar.
	// POST: devuelve true si se encuentra o false en caso contrario
	bool buscar(Tipo valor);

	// Verifica si una lista se encuentra vacía.
	// POST: Devuelve verdadero si la lista se encuentra vacía o falso en 
	// caso contrario.
	bool estaVacia();

	// Operador []
	// Permite acceder a los índices de la lista mediante la notación 
	// lista[i], donde i es un número entero comprendido entre [0, n-1],
	// siendo n el tamanio de la lista.
	Tipo operator[] (const size_t indice);

	// Asigna el contenido de una lista a otra
	Lista& operator= (const Lista& lista);
};




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
template <typename Tipo >
Lista< Tipo >::Lista() { }


// Constructor copia
template <typename Tipo >
Lista< Tipo >::Lista(const Lista< Tipo>& l) {
	this->lista = l.lista;
}

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

template <typename Tipo >
void Lista< Tipo >::ordenar() {
	Lock l(m);
	this->lista.sort();
}


// Busca un elemento en la lista.
// PRE: 'valor' es el valor del elemento a buscar.
// POST: devuelve true si se encuentra o false en caso contrario
template <typename Tipo >
bool Lista< Tipo >::buscar(Tipo valor) {
	Lock l(m);

	// Buscamos elemento en lista interna
	typename std::list< Tipo >::iterator i;
	i = std::find(this->lista.begin(), this->lista.end(), valor);

	// Devolvemos el resultado de la busqueda
	if(i != this->lista.end()) return true;
	return false;
}


// Verifica si una lista se encuentra vacía.
// POST: Devuelve verdadero si la lista se encuentra vacía o falso en 
// caso contrario.
template <typename Tipo >
bool Lista< Tipo >::estaVacia() {
	Lock l(m);
	return this->lista.empty();
}


// Operador []
// Permite acceder a los índices de la lista mediante la notación lista[i], 
// donde i es un número entero comprendido entre [0, n-1], siendo n el tamaño
// de la lista.
template <typename Tipo >
Tipo Lista< Tipo >::operator[] (const size_t indice) {	
	Lock l(m);

	// Corroboramos que no este vacia
	if(this->lista.empty())
		throw "ERROR: Se intenta acceder a una lista que se encuentra vacia.";
	// Corroboramos que el índice sea válido
	else if(indice >= this->lista.size())
		throw "ERROR: Indice de lista inválido.";
		
	// Creamos iterador y nos posicionamos en el índice deseado
	typename std::list< Tipo >::const_iterator it = this->lista.begin();
	for(size_t i = 0; i < indice; i++) ++it;

	return *it;
}


// Asigna el contenido de una lista a otra
template <typename Tipo >
Lista< Tipo >& Lista< Tipo >::operator= (const Lista< Tipo >& l) {
	this->lista = l.lista;
	return(*this);
}

#endif
