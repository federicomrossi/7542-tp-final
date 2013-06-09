// 
//  common_socket.cpp
//  CLASE SOCKET
// 
//  Clase que implementa la interfaz de los sockets de flujo (utilizando el 
//  protocolo TCP), proporcionando un conjunto medianamente extenso de métodos
//  y propiedades para las comunicaciones en red. 
// 


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>

#include "common_socket.h"




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor.
Socket::Socket() : activo(false) { }


// Constructor privado.
// Crea un nuevo socket.
// PRE: 'sockfd' es un filedescriptor que identifica a un socket.
Socket::Socket(const int sockfd) : sockfd(sockfd), activo(true) { }


// Destructor. 
// Cierra el socket.
Socket::~Socket() { 
	if(close(this->sockfd) == -1)
		std::cerr << "ERROR: No se ha podido cerrar el socket." << std::endl;
}


// Crea el socket
// POST: lanza una excepción si no se logra llevar a cabo la creación.
void Socket::crear() {
	if((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw "ERROR: No se ha podido crear el socket.";

	// Cambiamos el estado del socket
	this->activo = true;
}


// Conecta el socket a una dirección y puerto destino.
// PRE: 'hostDestino' es una cadena que contiene el nombre del host o la
// dirección IP a la que se desea conectar; 'puertoDestino' es el puerto 
// al que se desea conectar.
// POST: determina dirección y puertos locales si no se utilizó el método 
// bind() previamente. Además, lanza una excepción si no se pudo llevar a
// cabo la conexión.
void Socket::conectar(std::string hostDestino, int puertoDestino) {
	// Obtenemos host
	struct hostent *he = gethostbyname(hostDestino.c_str());

	// Cargamos datos de la conexión a realizar
	destinoDir.sin_family = AF_INET;
	destinoDir.sin_port = htons(puertoDestino);
	// destinoDir.sin_addr.s_addr = inet_addr(ipDestino.c_str());
	destinoDir.sin_addr = *((struct in_addr *)he->h_addr);
	memset(&(destinoDir.sin_zero), '\0', sizeof(destinoDir.sin_zero));

	// Conectamos
	if(connect(this->sockfd, (struct sockaddr *)&destinoDir, 
		sizeof(struct sockaddr)) == -1)
		throw "ERROR: No se pudo llevar a cabo la conexión.";
}


// Configura el socket para recibir conexiones en la dirección y puerto 
// previamente asociados mediante el método enlazar();
// PRE: 'maxConexiones' es el número de conexiones entrantes permitidas en
// la cola de entrada.
// POST: lanza una excepción si no se pudo inicializar la escucha.
void Socket::escuchar(int maxConexiones, int puerto, std::string ip) {
	// Enlazamos
	enlazar(puerto, ip);

	// Comenzamos la escucha
	if(listen(this->sockfd, maxConexiones) == -1)
		throw "ERROR: No se pudo comenzar a escuchar.";
}


// Espera una conexión en el socket previamente configurado con el método
// escuchar().
// POST: lanza una excepción si no pudo aceptar la conexión.
Socket* Socket::aceptar() {
	unsigned sin_size = sizeof(struct sockaddr_in);
	int sCliente = accept(sockfd, (struct sockaddr *)&destinoDir, &sin_size);

	// Corroboramos si no se cerró el socket
	if(!this->estaActivo()) return 0;
	// Corroboramos si se produjo un error
	else if (sCliente < 0)
		throw "ERROR: No se pudo aceptar la conexión";

	return (new Socket(sCliente));
}


// Envía datos a través del socket de forma completa.
// PRE: 'dato' es el dato que se desea enviar; 'longDato' es la longitud 
// de los datos en bytes.
// POST: devuelve 0 si se ha realizado el envio correctamente o -1 en caso
// de error.
int Socket::enviar(const void* dato, int longDato) {
	// Cantidad de bytes que han sido enviados
	int bytesTotal = 0;
	// Cantidad de bytes que faltan enviar
	int bytesRestantes = longDato;
	// Variable auxiliar
	int n = 0;

	while(bytesRestantes > 0) {
		// Realizamos envío de bytes
		n = send(this->sockfd, (char *) dato + bytesTotal, bytesRestantes, 0);
		
		// En caso de error, salimos
		if(n == -1)	break;	

		// Incrementamos la cantidad de bytes ya enviados
		bytesTotal += n;
		
		// Decrementamos cantidad de bytes restantes
		bytesRestantes -= n;
	}

	return (n == -1) ? -1:0;
}


// Recibe datos a través del socket.
// PRE: 'buffer' es el buffer en donde se va a depositar la información 
// leida; 'longBuffer' es la longitud máxima del buffer.
// POST: devuelve el número de bytes que han sido leidos o 0 (cero) si el
// host remoto a cerrado la conexión.
int Socket::recibir(void* buffer, int longBuffer) {
	// Limpiamos buffer
	memset(buffer, '\0', longBuffer);
	// Recibimos datos en buffer
	return recv(this->sockfd, buffer, longBuffer, 0);
}


// Cierra el socket. Brinda distintos tipos de formas de cerrar permitiendo
// realizar un cierre del envío y recepción de datos en forma controlada.
// PRE: si 'modo' es 0, no se permite recibir más datos; si es 1, no se 
// permite enviar más datos; si es 2, no se permite enviar ni recibir más
// datos, quedando inutilizable el socket. Si no se especifica ningún modo 
// al llamar al método, se utiliza por defecto el modo 2.
// POST: el socket quedará parcial o completamente inutilizable 
// dependiendo del modo elegido.
int Socket::cerrar(int modo) {
	if(modo == 2)
		this->activo = false;
		
	return shutdown(this->sockfd, modo);
}


// Corrobora si el socket se encuentra activo. Que no este activo significa
// da cuenta de que el socket se encuentra inutilizable para la transmisión
// y recepción de datos.
// POST: devuelve true si el socket se encuentra activo o false en su
// defecto. 
bool Socket::estaActivo() {
	return this->activo;
}


// Enlaza (asocia) al socket con un puerto y una dirección IP.
// PRE: 'ip' es una cadena que contiene el nombre del host o la dirección
// IP a la que se desea asociar; 'puerto' es el puerto al que se desea
// enlazar.
// POST: lanza una excepción si no se logra llevar a cabo el enlace.
void Socket::enlazar(int puerto, std::string ip) {
	int yes = 1;
	
	// Reutilizamos socket
	if(setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) 
		== -1)
		throw "ERROR: Antes de enlazar, no se pudo reutilizar socket.";

	// Cargamos datos del enlace a realizar
	this->miDir.sin_family = AF_INET;
	this->miDir.sin_port = htons(puerto);

	// Obtenemos host
	if(ip == "")
		this->miDir.sin_addr.s_addr = htonl(INADDR_ANY);
	else {
		struct hostent *he = gethostbyname(ip.c_str());
		this->miDir.sin_addr = *((struct in_addr *)he->h_addr);
	}

	memset(miDir.sin_zero, '\0', sizeof(miDir.sin_zero));

	// Enlazamos
	if(bind(this->sockfd, (struct sockaddr *)&miDir, sizeof(miDir)) < 0)
		throw "ERROR: No se pudo llevar a cabo el enlace.";
}
