#pragma once

#include "Utilities/Pair.h"
#include "../Renderer/Utilities/Color.h"

class Waves
{
private:
	Color ** _background;	//Matriz de colores del fondo
	int _energyWave;		//Energia de la onda (31/32)
	int _minFramesBetweenWaves, _maxFramesBetweenWaves;
	int width, height;		//Dimensiones de la onda

	//Estado
	int* buffer1;			//Alturas
	int* buffer2;			//Alturas

	const Pair dirs[4] = { {1,0},{0,1},{-1,0},{0,-1} };

public:
	Waves(int energyWave, int minFramesBetweenWaves, int maxFramesBetweenWaves, Color ** background);
	~Waves();

	/*
		Llama a UpdatePixel de todos los puntos de la onda
	*/
	void Update(int delta);

	/*
		Llama a render de todos los pixeles de la onda
	*/
	void Render();

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
		Devuelve si una posición no se sale de los límites de la ventana
	*/
	bool Correct(int x, int y);

	/*
		Restringe un valor al rango
	*/
	int Clamp(int min, int max, int value);
};

