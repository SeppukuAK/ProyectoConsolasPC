#pragma once
#include "../Renderer/Utilities/Color.h"

/*
	Objeto fuego. Tiene metodos para su actualización y dibujado
*/
class Fire
{
private:
	int _width, _height;	//Dimensiones del fuego
	Color* _palette;		//Colores del fuego

	int** matrix;			//Matriz con las temperaturas del fuego
	int posX, posY;			//Offset de posición del fuego
	int maxTemp;			//Temperatura máxima del fuego

public:
	/*
		Inicializa
	*/
	Fire(int width, int height, Color* palette, int numColors);

	/*
		Libera memoria
	*/
	~Fire();

	/*
		Hace avanzar el mapa de temperatura en función de si esta encendido o apagado
	*/
	void Update(bool on);

	/*
		Llama a render de todos los pixeles del fuego
	*/
	void Render();

private:
	/*
		Inicializa variables
	*/
	void Init();

	/*
		Evoluciona un pixel, actualizando su temperatura.
	*/
	void UpdatePixel(int x, int y);
};

