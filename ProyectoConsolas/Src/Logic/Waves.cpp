#include "Waves.h"
#include "../Renderer/Renderer.h"
#include <stdlib.h>		/* abs */
#include <iostream>		/* cout */

Waves::Waves(int energyWave, int minFramesBetweenWaves, int maxFramesBetweenWaves, Color ** background) :
	_energyWave(energyWave), _minFramesBetweenWaves(minFramesBetweenWaves), _maxFramesBetweenWaves(maxFramesBetweenWaves), _background(background)
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

}

void Waves::Update(int delta)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//Se suma cada una de las direcciones multiplicadas por 0.5
			int sumHeight = 0;
			for (auto dir : dirs)
			{
				int adjX = j + dir.First;
				int adjY = i + dir.Second;
				if (Correct(adjX, adjY))
					//sumHeight = old[adjY * width + adjX] * 0.5
					sumHeight += buffer1[adjY * width + adjX];	//Aritmetica entera
			}

			sumHeight = sumHeight >> 1;

			//Al resultado se le suma el valor de hace 2 frames
			buffer2[i*width + j] = sumHeight - buffer2[i*width + j];

			//Le reducimos un poco la altura por la perdida de energia
			// v *= _energyWave (31/32)
			buffer2[i*width + j] -= (buffer2[i*width + j] >> 5);	//Aritmetica entera
		}
	}

	int * temp = buffer1;
	buffer1 = buffer2;
	buffer2 = temp;

	if (delta % _minFramesBetweenWaves == 0)
	{
		buffer2[400 * width + 500] = 5000;
	}
}

void Waves::Render()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//Solo se modifica el color si ha cambiado la altura
			if (buffer2[i*width + j] != buffer1[i*width + j])
			{
				Color color = _background[i][j];

				int left = 0;
				int right = 0;

				if (Correct(j - 1, i))
					left = buffer2[i*width + j - 1];

				if (Correct(j + 1, i))
					right = buffer2[i*width + j + 1];

				int heightDiff = left - right;

				//Se obtiene el color en función de la altura de la onda y la imagen de fondo
				color.R = Clamp(0, 255, (color.R - heightDiff));
				color.G = Clamp(0, 255, (color.G - heightDiff));
				color.B = Clamp(0, 255, (color.B - heightDiff));

				Renderer::PutPixel(j, i, color);
			}
		}
	}
}

void Waves::UpdatePixel(int x, int y)
{
	////Elegimos de forma aleatoria que dirección escoge
	//int randomDir; //Coge un valor entre [-1,+1]

	////Ajuste para que no se salga por los lados
	//if (x == 0)
	//	randomDir = rand() % 2;			//[0,1]
	//else if (x == _width - 1)
	//	randomDir = rand() % 2 - 1;		//[-1,0]
	//else
	//	randomDir = rand() % 3 - 1;		//[-1,1]

	////Se elige de forma aleatoria si se enfria o no
	//int randomCooling = rand() % 2; //[0,1]

	//matrix[y][x] = matrix[y + 1][x + randomDir] - randomCooling;

	////Ajuste para que nunca sea menor que 0
	//if (matrix[y][x] < 0)
	//	matrix[y][x] = 0;

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