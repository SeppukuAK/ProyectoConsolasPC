#pragma once
#include "../Utilities/Color.h"

/*
	Clase con métodos estáticos. Maneja el renderBuffer
*/
class RendererPS4
{
private:
	static int numBuffers;			//Número de frame buffers del Renderer
	static int width;				//Ancho de la ventana
	static int height;				//Alto de la ventana

public:

	static void Init();
	static void Release();

	/*
		Limpia la pantalla con el color especificado
	*/
	static void Clear(Color color);

	/*
		Pinta un pixel en el RenderBuffer con el color especificado en la posición (x,y).
		No aparece en pantalla hasta que se hace Present().
	*/
	static void PutPixel(int x, int y, Color color);

	/*
		Realiza el intercambio de Buffers: Muestra en pantalla el siguiente RenderBuffer.
	*/
	static void Present();

	//Getters

	static int GetWidth() { return width; };
	static int GetHeight() { return height; };

	static int GetNumBuffers() { return numBuffers; };
};