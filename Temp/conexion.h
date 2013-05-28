/*
 * conexion.h
 *
 *  Created on: 27/05/2013
 *      Author: fiona
 */

#ifndef CONEXION_H_
#define CONEXION_H_

#include "gtkmm.h"


class conexion {
private:
	Gtk::Window* main;
	Gtk::Button *botonConectar;
	Gtk::Button *botonSalir;
	Gtk::Entry 	*usuarioTextBox;
	Gtk::Entry  *passTextBox;

public:
	conexion();
	virtual ~conexion();
	void correr();

protected:
	void on_buttonConectar_clicked();
	void on_buttonSalir_clicked();
};

#endif /* CONEXION_H_ */
