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

void RendererPC::Clear(Color color)
{
	//Establecemos un color de dibujado
	SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);

	//Limpia la pantalla con ese color
	SDL_RenderClear(renderer);
}

void RendererPC::PutPixel(int x, int y, Color color)
{
	//Establecemos un color de dibujado
	SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);

	//Pintamos el pixel
	SDL_RenderDrawPoint(renderer, x, y);
}

void RendererPC::DrawSquare(int x, int y, Color color)
{
	for (size_t i = 0; i < _scale; i++)
	{
		for (size_t j = 0; j < _scale; j++)
		{
			PutPixel(x + j, y + i, color);

		}

	}
}

void RendererPC::DrawRect(Image* image, int posX, int posY, Rect sRect)
{
	int imageWidth = image->GetWidth();

	Color* colorArray = image->GetColorArray();

	for (int i = 0; i < sRect.Height; i++)
	{
		for (int j = 0; j < sRect.Width; j++)
		{
			DrawSquare((posX + j) * _scale, (posY + i) * _scale, colorArray[(i + sRect.Y) * imageWidth + (j + sRect.X)]);
		}
	}

}

void RendererPC::Present()
{
	//Actualiza la pantalla
	SDL_RenderPresent(renderer);
}


#endif