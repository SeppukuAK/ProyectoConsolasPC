
//Se envuelve todo para evitar que se compile si no estamos en la plataforma
#if PLATFORM_PC

#include "RendererPC.h"
#include <SDL.h>		/* SDL. Pintado */
#include <stdio.h> 
#include "../Color.h"
#include "../Image.h"
#include "../../Utilities/Rect.h"

//Inicialización de atributos estáticos
int RendererPC::_screenWidth = 0;
int RendererPC::_screenHeight = 0;
int RendererPC::_numBuffers = 0;
int RendererPC::_scale = 1;

SDL_Window* RendererPC::window = NULL;
SDL_Renderer* RendererPC::renderer = NULL;

void RendererPC::Init(int screenWidth, int screenHeight, int numBuffers)
{
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_numBuffers = numBuffers;

	//Crea la ventana en función del modo establecido. Fullscreen 2 buffer. Windowed 1 buffer.
	if (_numBuffers == 2)
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
	else if (_numBuffers == 1)
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	else
		printf("Error: En PC solo se dispone de 1 o 2 RenderBuffer");

	if (window == NULL)
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());

	//Crea el renderer para la ventana
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//Caso de error
	if (renderer == NULL)
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());

	//Si no ha habido error. Inicializa el color del render
	else
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

}

void RendererPC::Release()
{
	//Destruye el renderer
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	//Destrucción de la ventana
	SDL_DestroyWindow(window);
	window = NULL;
}

void RendererPC::Clear(const Color& color)
{
	//Establecemos un color de dibujado
	SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);

	//Limpia la pantalla con ese color
	SDL_RenderClear(renderer);
}

void RendererPC::PutPixel(const int& x, const int& y, const Color& color)
{
	//Establecemos un color de dibujado
	SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);

	//Pintamos el pixel
	SDL_RenderDrawPoint(renderer, x, y);
}

void RendererPC::DrawSquare(const int& posX, const int& posY, const Color& color)
{
	//Establecemos un color de dibujado
	SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);

	for (int i = 0; i < _scale; i++)
	{
		int y = posY + i;
		for (int j = 0; j < _scale; j++)
		{
			int x = posX + j;

			//Pintamos el pixel
			SDL_RenderDrawPoint(renderer, x, y);
		}

	}
}

void RendererPC::DrawRect(Image* image, const int& posX, const int& posY, const Rect& sRect)
{
	int imageWidth = image->GetWidth();
	Color* colorArray = image->GetColorArray();

	for (int i = 0; i < sRect.Height; i++)
	{
		int y = (posY + i) * _scale;
		int yColor = (i + sRect.Y);
		for (int j = 0; j < sRect.Width; j++)
		{
			int x = (posX + j) * _scale;
			int xColor = (j + sRect.X);
			DrawSquare(x, y, colorArray[yColor * imageWidth + xColor]);
		}
	}

}

void RendererPC::Present()
{
	//Actualiza la pantalla
	SDL_RenderPresent(renderer);
}

#endif