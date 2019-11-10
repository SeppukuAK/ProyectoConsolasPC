//Se envuelve todo para evitar que se compile si no estamos en la plataforma
#if PLATFORM_PC

#include "RendererPC.h"
#include <stdio.h> 
#include "../../Platform/Platform.h"

//Inicialización de atributos estáticos
int RendererPC::numBuffers = 0;
int RendererPC::width = 0;
int RendererPC::height = 0;
SDL_Renderer* RendererPC::renderer = NULL;

void RendererPC::Init()
{
	//Obtiene referencia a la ventana
	SDL_Window* window = Platform::GetWindow();

	//Crea el renderer para la ventana
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//Caso de error
	if (renderer == NULL)
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());

	//Si no ha habido error
	else
	{
		//Inicializa el color del render
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

		//Obtenemos el tamaño de la ventana
		SDL_GetWindowSize(window, &width, &height);

		//Si es fullscreen tiene 2 buffers
		if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN)
			numBuffers = 2;
		else
			numBuffers = 1;

	}
}

void RendererPC::Release()
{
	//Destruye el renderer
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
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
	//TODO: QUIZA HAY QUE COMPROBAR SI EL COLOR COINCIDE CON EL QUE TENEMOS

	//Establecemos un color de dibujado
	SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);

	//Pintamos el pixel
	SDL_RenderDrawPoint(renderer, x, y);
}

void RendererPC::Present()
{
	//Actualiza la pantalla
	SDL_RenderPresent(renderer);
}

#endif