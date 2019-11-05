#include "../Utilities/Color.h"
#include <SDL.h>



//Utiliza la librería SDL: permite la carga de pngs y el pintado de píxeles
class RendererPC
{

private:

	//Tengo que tener un atributo que sea un frameBuffer de 800x600 o lo que sea (matriz)

	static int numBuffers; //número de frame buffers del Renderer
	static int width;
	static int height;
	static SDL_Renderer* renderer;


public:

	//RendererPC() { renderer = NULL; };
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
		Espera o no dependiendo de como se ha inicializado. De forma normal espera.
	*/
	static void Present();

	//TODO: Utilizaremos estos dos metodos para dibujar el fuego centrado en la parte inferior.
	inline static int GetWidth() { return width; };
	inline static int GetHeight() { return height; };

	inline static int GetNumBuffers() { return numBuffers; };
};
SDL_Renderer* RendererPC::renderer = NULL;

