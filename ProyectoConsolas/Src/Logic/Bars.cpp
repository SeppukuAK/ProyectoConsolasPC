#include "Bars.h"
#include "../Renderer/Renderer.h"

Bars::Bars(int barWidth, int barHeight, int barHoriDist, int barVertDist, Color barColor, Color clearColor, int fireHeight)
	: _barWidth(barWidth), _barHeight(barHeight), _barColor(barColor), _clearColor(clearColor)
{
	screenWidth = Renderer::GetWidth();
	diffHoriBars = _barWidth + barHoriDist;
	diffVertBars = _barHeight + barVertDist;

	numHoriBars = screenWidth / diffHoriBars;
	numVertBars = (Renderer::GetHeight() - fireHeight) / diffVertBars;
}

Bars::~Bars()
{
}

void Bars::Draw(int x)
{
	//Pinta todas las barras a partir de X
	for (int r = 0; r < numVertBars; r++)
	{
		int rowOffset = r * diffVertBars;
		for (int c = 0; c < numHoriBars; c++)
		{
			int columnOffset = x + c * diffHoriBars;
			for (int i = 0; i < _barHeight; i++)
			{
				int y = i + rowOffset;
				for (int j = 0; j < _barWidth; j++)
					Renderer::PutPixel(j + columnOffset, y, _barColor);
			}
		}
	}
}

void Bars::DrawWithDelta(int x, int delta)
{
	//Pintar cada fila
	for (int r = 0; r < numVertBars; r++)
	{
		int rowOffset = r * diffVertBars;

		//Pintar cada columna
		for (int c = 0; c < numHoriBars; c++)
		{
			int columnOffset = x + c * diffHoriBars;

			//Borramos las columnas de la izquierda antiguas del buffer
			for (int b = 0; b < delta; b++)
			{
				int x = (columnOffset - (b + 1)) % screenWidth;
				for (int i = 0; i < _barHeight; i++)
					Renderer::PutPixel(x, i + rowOffset, _clearColor);
			}

			//Pintamos las columnas del nuevo buffer por la derecha
			for (int b = 0; b < delta; b++)
			{
				int x = (columnOffset - (b + 1) + _barWidth) % screenWidth;
				for (int i = 0; i < _barHeight; i++)
					Renderer::PutPixel(x, i + rowOffset, _barColor);
			}
		}
	}
}
