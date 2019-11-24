#pragma once

#include "Utilities/Pair.h"
#include "../Renderer/Utilities/Color.h"

class Waves
{
private:
	Color ** _background;	//Matriz de colores del fondo
	int _energyWave;		//Energia de la onda (31/32)
	int _heightWave;		//Altura de la onda
	int _minFramesBetweenWaves, _maxFramesBetweenWaves;
	int width, height;		//Dimensiones de la onda
	int nextWaveFrame;		//Frame en el que suceder� la proxima onda

	//Estado
	int* buffer1;			//Alturas
	int* buffer2;			//Alturas

	const Pair dirs[4] = { {1,0},{0,1},{-1,0},{0,-1} };

public:
	Waves(int energyWave, int heightWave, int minFramesBetweenWaves, int maxFramesBetweenWaves, Color ** background);
	~Waves();

	/*
		Llama a UpdatePixel de todos los puntos de la onda
	*/
	void Update(int delta);

	/*
		Llama a render de todos los pixeles de la onda
	*/
	void Draw();

	/*
		Llama a render de todos los pixeles de la onda
	*/
	void DrawWithDelta(int delta);

private:
	/*
		Inicializa variables
	*/
	void Init();

	/*
		Evoluciona un pixel, actualizando su altura.
	*/
	void UpdatePixel(int x, int y);

	/*
		Dibuja un pixel, en funci�n de su altura
	*/
	void DrawPixel(int x, int y);

	/*
		Devuelve si una posici�n no se sale de los l�mites de la ventana
	*/
	bool Correct(int x, int y);

	/*
		Restringe un valor al rango
	*/
	int Clamp(int min, int max, int value);
};

