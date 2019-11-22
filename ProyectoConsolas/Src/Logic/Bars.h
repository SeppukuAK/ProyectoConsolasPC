#pragma once
#include "../Renderer/Utilities/Color.h"

/*
	Conjunto de barras blancas. Tiene metodos para su dibujado.
*/
class Bars
{
private:
	int _barWidth, _barHeight;				//Dimensiones de cada barra
	Color _barColor, _clearColor;			//Colores

	int numHoriBars, numVertBars;			//Número total de barras
	int diffHoriBars, diffVertBars;			//Diferencia entre el inicio de cada barra y la siguiente
	int screenWidth;						//Ancho de la pantalla

public:
	/*
		Inicializa
	*/
	Bars(int barWidth, int barHeight, int barHoriDist, int barVertDist, Color barColor, Color clearColor, int fireHeight);

	/*
		Libera memoria
	*/
	~Bars();

	/*
		Pinta todos los pixeles de las barras a partir de la pos X
	*/
	void Draw(int x);

	/*
		Pinta todos los pixeles de las barras a partir de la pos 'X' y borra las 'delta' columnas anteriores
		delta es el número de frameBuffers
		Aprovecha la coherencia entre frames para no hacer clear todo el rato
		Borra las columnas izquierda de cada barra y pinta las columnas derecha de cada barra
	*/
	void DrawWithDelta(int x, int delta);
};

