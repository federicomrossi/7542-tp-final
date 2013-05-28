#ifndef COMMON_COLA_H_
#define COMMON_COLA_H_

#include "common_mutex.h"
#include "common_lock.h"
#include <queue>

template <class T>
class Cola {
private:
	std::queue<T>* cola;
	Mutex* mutex;
public:
	Cola() {
		this->cola = new std::queue<T>;
		this->mutex = new Mutex;
	};

	virtual ~Cola() {
		if (this->cola) {
			delete(this->cola);
			this->cola = NULL;
		}
		if (this->mutex) {
			delete(this->mutex);
			this->mutex = NULL;
		}
	};

	void push(const T &dato) {
		Lock lock(*this->mutex);
		cola->push(dato);
		lock.signal();
	};

	void pop() {
		Lock lock(*this->mutex);
		cola->pop();
	};

	T& front() {
		Lock lock(*this->mutex);
		return (cola->front());
	};

	int tamanio() {
		Lock lock(*this->mutex);
		return (cola->size());
	};

	T pop2() {
		Lock lock(*this->mutex);
		if (cola->size() == 0)
			lock.wait();
		T t = cola->front();
		cola->pop();
		return t;
	};

	bool vacia() {
		Lock lock(*this->mutex);
		return(cola->empty());
	}
};

#endif /* COMMON_COLA_H_ */
