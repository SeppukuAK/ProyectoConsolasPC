#include "../Utilities/Color.h"

class RendererPS4
{
	//Los métodos son estáticos

	static void Init();
	static void Release();

	static void Clear(Color color);

	/*
		Pone un Pixel en el RenderBuffer. No aparece en pantalla hasta que se hace present
	*/
	static void PutPixel(int x, int y, Color color);

	/*
		Muestra en pantalla el RenderBuffer.
	*/
	static void Present();

	//TODO: Utilizaremos estos dos metodos para dibujar el fuego centrado en la parte inferior.
	static int GetWidth();
	static int GetHeight();

};