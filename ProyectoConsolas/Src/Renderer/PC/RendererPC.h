#pragma once

//Declaraci�n adelantada para evitar inclusi�n adicional
class SDL_Window;
class SDL_Renderer;
struct Color;
class Image;
struct Rect;

/*
	Clase con m�todos est�ticos. Maneja el renderBuffer
*/
class RendererPC
{
private:
	static int _screenWidth;		//Ancho de la ventana
	static int _screenHeight;		//Alto de la ventana
	static int _numBuffers;			//N�mero de frame buffers del Renderer
	static int _scale;				//Escala que se aplica al pintado de Sprites

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
	static void Clear(const Color& color);

	/*
		Pinta un pixel en el RenderBuffer con el color especificado en la posici�n (x,y).
		No aparece en pantalla hasta que se hace Present().
	*/
	static void PutPixel(const int& x, const int& y, const Color& color);

	/*
		Pinta un cuadrado de pixeles en funci�n de la escala
	*/
	static void DrawSquare(const int& posX, const int& posY, const Color& color);

	/*
		Pinta una seccion de una imagen en la posici�n especificada
	*/
	static void DrawRect(Image* image, const int& posX, const int& posY, const Rect& sRect);

	/*
		Realiza el intercambio de Buffers: Muestra en pantalla el siguiente RenderBuffer.
	*/
	static void Present();

	//Setters
	inline static void SetScale(const int& newScale) { _scale = newScale; }

	//Getters
	inline static int GetWidth() { return _screenWidth; };
	inline static int GetHeight() { return _screenHeight; };
	inline static int GetNumBuffers() { return _numBuffers; };
};


