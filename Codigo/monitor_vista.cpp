#include "monitor_vista.h"
#include "monitor_canvas.h"
#include <iostream>
Vista::Vista()
{
}

Vista::~Vista()
{
}

void Vista::setCanvas(Canvas* canvas) {
	this->canvas = canvas;
}
