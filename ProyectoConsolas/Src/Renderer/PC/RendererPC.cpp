//Se envuelve todo para evitar que se compile si no estamos en la plataforma
#if PLATFORM_PC
#include "RendererPC.h"
#include <stdio.h> 

//TODO: Implementar los métodos
void RendererPC::Init() 
{
	//Create renderer for window
	
	//renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		//Initialize renderer color
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	}
	return;
}
void RendererPC::Release() {}

/*Limpia la pantalla con el color especificado*/
void RendererPC::Clear(Color color)
{
	//Establecemos un color de dibujado
	SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);

	//Limpia la pantalla 
	SDL_RenderClear(renderer);
}

/*Se encarga de pintar un pixel en pantalla con el color especificado en la posición (x,y)*/
void RendererPC::PutPixel(int x, int y, Color color) 
{
	//TODO: QUIZA HAY QUE COMPROBAR SI EL COLOR COINCIDE CON EL QUE TENEMOS
	//Establecemos un color de dibujado
	SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);

	//Pintamos el pixel de ese color
	SDL_RenderDrawPoint(renderer, x, y);
}
//
/*Realiza el intercambio de buffers*/
void RendererPC::Present() 
{
	//Actualiza la pantalla
	SDL_RenderPresent(renderer);
}

#endif