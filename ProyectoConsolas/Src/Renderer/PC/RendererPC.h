#pragma once

//Declaración adelantada para evitar inclusión adicional
class SDL_Window;
class SDL_Renderer;
struct Color;
class Image;
struct Rect;

/*
	Clase con métodos estáticos. Maneja el renderBuffer
*/
class RendererPC
{
private:
	static int _screenWidth;		//Ancho de la ventana
	static int _screenHeight;		//Alto de la ventana
	static int _numBuffers;			//Número de frame buffers del Renderer
	static int _scale;

	static SDL_Window* window;		//Referencia a la ventana de SDL
	static SDL_Renderer* renderer;	//Referencia al renderer de SDL

public:

	/*
		 Crea la ventana con el modo y las dimensiones pasadas y el renderer
	*/
	static void Init(int screenWidth, int screenHeight, int numBuffers);

	/*
		Destruye la ventana y el renderer
	*/
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

	static void DrawSquare(int x, int y, Color color);

	/*
		Pinta una seccion de una imagen en la posición especificada
	*/
	static void DrawRect(Image* image, int posX, int posY, Rect sRect);

	/*
		Realiza el intercambio de Buffers: Muestra en pantalla el siguiente RenderBuffer.
	*/
	static void Present();

	inline static void SetScale(int newScale) { _scale = newScale; }

	//Getters
	inline static int GetWidth() { return _screenWidth; };
	inline static int GetHeight() { return _screenHeight; };
	inline static int GetNumBuffers() { return _numBuffers; };
};


