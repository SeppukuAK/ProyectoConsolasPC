#include "Waves.h"
#include "../Renderer/Renderer.h"
#include <stdlib.h>		/* srand, rand */
#include <iostream>		/* cout */

Waves::Waves(int energyWave, int heightWave, int minFramesBetweenWaves, int maxFramesBetweenWaves, Color** background) :
	_energyWave(energyWave), _heightWave(heightWave), _minFramesBetweenWaves(minFramesBetweenWaves),
	_maxFramesBetweenWaves(maxFramesBetweenWaves), _background(background)
{
	Init();
}

Waves::~Waves()
{
	//Limpiamos memoria
	delete[] buffer1;
	delete[] buffer2;
}

void Waves::Init()
{
	//Calculo del offset de posición del fuego
	width = Renderer::GetWidth();
	height = Renderer::GetHeight();

	int bufferSize = width * height;

	//Cada valor representa la altura de la onda
	buffer1 = new int[bufferSize];
	buffer2 = new int[bufferSize];
	for (int i = 0; i < bufferSize; ++i)
	{
		buffer1[i] = 0;
		buffer2[i] = 0;
	}

	nextWaveFrame = _minFramesBetweenWaves + (rand() % (_maxFramesBetweenWaves - _minFramesBetweenWaves));
}

void Waves::Update(int delta)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			UpdatePixel(j, i);
		}
	}

	int* temp = buffer1;
	buffer1 = buffer2;
	buffer2 = temp;

	//Se genera onda
	if (delta == nextWaveFrame)
	{
		int x = rand() % width;
		int y = rand() % height;

		buffer2[y * width + x] = _heightWave;
		nextWaveFrame = delta + _minFramesBetweenWaves + (rand() % (_maxFramesBetweenWaves - _minFramesBetweenWaves));
	}
}

void Waves::Draw()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			DrawPixel(j, i);
		}
	}
}

void Waves::DrawWithDelta(int delta)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//Solo se modifica el color si ha cambiado la altura
			if (buffer2[i * width + j] != buffer1[i * width + j])
			{
				DrawPixel(j, i);
			}
		}
	}
}

void Waves::UpdatePixel(int x, int y)
{
	//Se suma cada una de las direcciones multiplicadas por 0.5
	int sumHeight = 0;
	for (auto dir : dirs)
	{
		int adjX = x + dir.First;
		int adjY = y + dir.Second;
		if (Correct(adjX, adjY))
			//sumHeight = old[adjY * width + adjX] * 0.5
			sumHeight += buffer1[adjY * width + adjX];	//Aritmetica entera
	}

	sumHeight = sumHeight >> 1;

	//Al resultado se le suma el valor de hace 2 frames
	buffer2[y * width + x] = sumHeight - buffer2[y * width + x];

	//Le reducimos un poco la altura por la perdida de energia
	// v *= _energyWave (31/32)
	buffer2[y * width + x] -= (buffer2[y * width + x] >> 5);	//Aritmetica entera
}

void Waves::DrawPixel(int x, int y)
{
	Color color = _background[y][x];

	int left = 0;
	int right = 0;

	if (Correct(x - 1, y))
		left = buffer2[y * width + x - 1];

	if (Correct(x + 1, y))
		right = buffer2[y * width + x + 1];

	int heightDiff = left - right;

	//Se obtiene el color en función de la altura de la onda y la imagen de fondo
	color.R = Clamp(0, 255, (color.R - heightDiff));
	color.G = Clamp(0, 255, (color.G - heightDiff));
	color.B = Clamp(0, 255, (color.B - heightDiff));

	Renderer::PutPixel(x, y, color);
}

bool Waves::Correct(int x, int y)
{
	return (x >= 0 && x < width && y >= 0 && y < height);
}

int Waves::Clamp(int min, int max, int value)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}