#include "Fire.h"
#include "../Renderer/Renderer.h"
#include <stdlib.h>		/* srand, rand */

Fire::Fire(int width, int height, Color* palette, int numColors)
	: _width(width), _height(height), _palette(palette)
{
	matrix = nullptr;
	maxTemp = numColors - 1;
	Init();
}

Fire::~Fire()
{
	//Limpiamos memoria
	for (int i = 0; i < _height; ++i)
		delete[] matrix[i];

	delete[] matrix;
}

void Fire::Update(bool on)
{
	int lastRowIndex = _height - 1;
	//Fuego encendido
	if (on)
	{
		//Va aumentando la temperatura de la linea de abajo aleatoriamente hasta que alcanza el tope
		for (int j = 0; j < _width; j++)
		{
			matrix[lastRowIndex][j] += rand() % 2;

			//Se ajusta a la temperatura maxima
			if (matrix[lastRowIndex][j] > maxTemp)
				matrix[lastRowIndex][j] = maxTemp;
		}
	}

	//Fuego apagado
	else
	{
		//Va disminuyendo la temperatura de la linea de abajo aleatoriamente hasta que alcanza el tope
		for (int j = 0; j < _width; j++)
		{
			matrix[lastRowIndex][j] -= rand() % 2;

			//Se ajusta a la temperatura minima
			if (matrix[lastRowIndex][j] < 0)
				matrix[lastRowIndex][j] = 0;
		}
	}

	//Recorremos de abajo a arriba excepto la linea de abajo del todo
	for (int i = lastRowIndex - 1; i >= 0; i--)
		for (int j = 0; j < _width; j++)
			UpdatePixel(j, i);
}

void Fire::Render()
{
	for (int i = 0; i < _height; i++)
	{
		int y = i + posY;
		for (int j = 0; j < _width; j++)
			Renderer::PutPixel(j + posX, y, _palette[matrix[i][j]]);
	}
}

void Fire::Init()
{
	//Cada valor representa la temperatura del fuego. Valores entre 0(frio) y sizeFirePalette(caliente):
	matrix = new int*[_height];
	for (int i = 0; i < _height; ++i)
	{
		matrix[i] = new int[_width];
		for (size_t j = 0; j < _width; j++)
			matrix[i][j] = 0;
	}

	//Calculo del offset de posición del fuego
	posX = Renderer::GetWidth() / 2 - _width / 2;
	posY = Renderer::GetHeight() - _height;
}

void Fire::UpdatePixel(int x, int y)
{
	//Elegimos de forma aleatoria que dirección escoge
	int randomDir; //Coge un valor entre [-1,+1]

	//Ajuste para que no se salga por los lados
	if (x == 0)
		randomDir = rand() % 2;			//[0,1]
	else if (x == _width - 1)
		randomDir = rand() % 2 - 1;		//[-1,0]
	else
		randomDir = rand() % 3 - 1;		//[-1,1]

	//Se elige de forma aleatoria si se enfria o no
	int randomCooling = rand() % 2; //[0,1]

	matrix[y][x] = matrix[y + 1][x + randomDir] - randomCooling;

	//Ajuste para que nunca sea menor que 0
	if (matrix[y][x] < 0)
		matrix[y][x] = 0;

}