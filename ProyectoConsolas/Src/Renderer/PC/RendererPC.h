#include "../Utilities/Color.h"

//Utiliza la librería SDL: permite la carga de pngs y el pintado de píxeles
class RendererPC
{
	//Los métodos son estáticos

	//TODO: Se inicializa en modo ventana o pantalla completa. 
	static void Init();
	static void Release();
	//TODO: SDL explica como hacer PutPixel, Clear y Present

	static void Clear(MyColor color);

	/*
		Pone un Pixel en el RenderBuffer. No aparece en pantalla hasta que se hace present
	*/
	static void PutPixel(int x, int y, MyColor color);

	/*
		Muestra en pantalla el RenderBuffer.
		Espera o no dependiendo de como se ha inicializado. De forma normal espera.
	*/
	static void Present();

	//TODO: Utilizaremos estos dos metodos para dibujar el fuego centrado en la parte inferior.
	static int GetWidth();
	static int GetHeight();

private:
	//Tengo que tener un atributo que sea un frameBuffer de 800x600 o lo que sea (matriz)
};

