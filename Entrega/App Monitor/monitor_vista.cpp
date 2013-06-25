#include "monitor_vista.h"
#include "monitor_graficador.h"
#include <iostream>
Vista::Vista()
{
}

Vista::~Vista()
{
}

void Vista::setGraficador(Graficador* graficador) {
	this->graficador = graficador;
}
