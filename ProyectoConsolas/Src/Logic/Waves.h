#pragma once

#include "Utilities/Pair.h"
#include "../Renderer/Utilities/Color.h"
#include "../Renderer/RendererThread.h"

class Waves
{
private:
	Color* _background;										//Matriz de colores del fondo
	int _heightWave;										//Altura de la onda
	int _minFramesBetweenWaves, _maxFramesBetweenWaves;		//Tiempo minimo y maximo entre 1 gota y la siguiente

	int width, height;										//Dimensiones de la onda
	int bufferSize;											//Tamaño de los buffers de altura
	int numBuffers;											//Número de buffers (uno más que renderBuffers)
	int nextWaveFrame;										//Frame en el que sucederá la proxima onda
	int bufferIndex;										//Indice del buffer actual
	RenderCommand renderCommand;

	//Estado
	int** buffers;			//Array de buffers(1-17)
	int* diffHeight;

	const Pair dirs[4] = { {1,0},{0,1},{-1,0},{0,-1} };		//Direcciones

public:
	Waves(Color* background, int heightWave, int minFramesBetweenWaves, int maxFramesBetweenWaves);
	~Waves();

	/*
		Llama a UpdatePixel de todos los puntos de la onda
	*/
	void Update(int delta);

	/*
		Devuelve el comando de pintado de la lluvia actual
	*/
	RenderCommand GetRenderCommand() { return renderCommand; };

private:
	/*
		Inicializa variables
	*/
	void Init();

	/*
		Evoluciona un pixel, actualizando su altura.
	*/
	void UpdatePixel(int x, int y);


	void UpdateRenderCommand(int delta);

	/*
		Devuelve si una posición no se sale de los límites de la ventana
	*/
	bool Correct(int x, int y);
};

