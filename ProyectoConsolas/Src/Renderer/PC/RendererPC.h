#pragma once
#include "../Utilities/Color.h"
#include "../Image.h"
#include <SDL.h> //Utiliza la librer�a SDL: permite la carga de pngs y el pintado de p�xeles

/*
	Clase con m�todos est�ticos. Maneja el renderBuffer
*/
class RendererPC
{
private:
	static int numBuffers;			//N�mero de frame buffers del Renderer
	static int width;				//Ancho de la ventana
	static int height;				//Alto de la ventana
	static SDL_Renderer* renderer;	//Referencia al renderer de SDL

public:

	/*
		Crea el renderer para la ventana y inicializa atributos
	*/
	static void Init();

	/*
		Destruye el renderer
	*/
	static void Release();

	/*
		Limpia la pantalla con el color especificado
	*/
	static void Clear(Color color);

	/*
		Pinta un pixel en el RenderBuffer con el color especificado en la posici�n (x,y).
		No aparece en pantalla hasta que se hace Present().
	*/
	static void PutPixel(int x, int y, Color color);

	/*
		Pinta una seccion de una imagen en la posici�n especificada
	*/
	static void DrawRect(Image* image, int posX, int posY, int sx1, int sy1, int sx2, int sy2);

	/*
		Realiza el intercambio de Buffers: Muestra en pantalla el siguiente RenderBuffer.
	*/
	static void Present();

	//Getters

	static int GetWidth() { return width; };
	static int GetHeight() { return height; };

	static int GetNumBuffers() { return numBuffers; };
};


