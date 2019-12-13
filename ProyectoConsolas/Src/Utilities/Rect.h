#pragma once

/*
	Rectángulo con origen en la esquina superior izquierda
*/
class Rect
{
	Rect(int x, int y, int width, int height) : _x(x), _y(y), _width(width), _height(height) {};

	int _x, _y;					//Coordenada de esquina superior izquierda
	int _width, _height;		//Dimensiones
};