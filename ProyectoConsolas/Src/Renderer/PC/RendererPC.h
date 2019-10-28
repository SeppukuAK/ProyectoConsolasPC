#include "../Utilities/Color.h"

//Utiliza la librería SDL: permite la carga de pngs y el pintado de píxeles
class RendererPC
{
	//Los métodos son estáticos

	//TODO: Se inicializa en modo ventana o pantalla completa. 
	static void Init();
	static void Release();
	//TODO: SDL explica como hacer PutPixel, Clear y Present

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

