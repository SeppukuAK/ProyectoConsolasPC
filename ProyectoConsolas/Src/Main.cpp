//#include "Renderer/Renderer.h"
//#include "Platform/Platform.h"
#include "Renderer/Utilities/Color.h"

//#include
//#include <iostream>

//Using SDL and standard IO 
#include <SDL.h>
#include <stdio.h> 
#include <iostream>

using namespace std;

//Screen dimension constants 
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#define RGB(r, g, b) { (Uint8)r, (Uint8)g, (Uint8)b,255 }

MyColor paletaFuego[] = {
				 RGB(0x00,0x00,0x00),
				 RGB(0x07,0x07,0x07),
				 RGB(0x1F,0x07,0x07),
				 RGB(0x2F,0x0F,0x07),
				 RGB(0x47,0x0F,0x07),
				 RGB(0x57,0x17,0x07),
				 RGB(0x67,0x1F,0x07),
				 RGB(0x77,0x1F,0x07),
				 RGB(0x8F,0x27,0x07),
				 RGB(0x9F,0x2F,0x07),
				 RGB(0xAF,0x3F,0x07),
				 RGB(0xBF,0x47,0x07),
				 RGB(0xC7,0x47,0x07),
				 RGB(0xDF,0x4F,0x07),
				 RGB(0xDF,0x57,0x07),
				 RGB(0xDF,0x57,0x07),
				 RGB(0xD7,0x5F,0x07),
				 RGB(0xD7,0x5F,0x07),
				 RGB(0xD7,0x67,0x0F),
				 RGB(0xCF,0x6F,0x0F),
				 RGB(0xCF,0x77,0x0F),
				 RGB(0xCF,0x7F,0x0F),
				 RGB(0xCF,0x87,0x17),
				 RGB(0xC7,0x87,0x17),
				 RGB(0xC7,0x8F,0x17),
				 RGB(0xC7,0x97,0x1F),
				 RGB(0xBF,0x9F,0x1F),
				 RGB(0xBF,0x9F,0x1F),
				 RGB(0xBF,0xA7,0x27),
				 RGB(0xBF,0xA7,0x27),
				 RGB(0xBF,0xAF,0x2F),
				 RGB(0xB7,0xAF,0x2F),
				 RGB(0xB7,0xB7,0x2F),
				 RGB(0xB7,0xB7,0x37),
				 RGB(0xCF,0xCF,0x6F),
				 RGB(0xDF,0xDF,0x9F),
				 RGB(0xEF,0xEF,0xC7),
				 RGB(0xFF,0xFF,0xFF)
};

void Logic(SDL_Renderer* renderer)
{
	//Clear screen
	SDL_RenderClear(renderer);


	cout << paletaFuego[3].G;
	//Dibujamos cuadrado
	for (int j = 0; j < 38 * 2; j++)
	{
		MyColor color = paletaFuego[j % sizeof(paletaFuego)];
		SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);
		for (size_t i = 0; i < 50; i++)
		{
			SDL_RenderDrawPoint(renderer, 50 + i, 50 + j);

		}
	}

	//Update screen
	SDL_RenderPresent(renderer);

	SDL_Delay(3000);
}


/*
Aplicación que simula un fuego en la parte inferior de la pantalla y junto con unas barras blancas
que se desplazan a la derecha en la parte superior
*/
int main(int argc, char* args[])
{
	//The window we'll be rendering to 
	SDL_Window* window = NULL;

	//The window renderer
	SDL_Renderer* renderer = NULL;

	//Initialize SDL 
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

	else
	{

		bool success;
		//Create window 
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}

		else
		{
			//Create renderer for window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

			if (renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}

		}
	}


	//---------------------
	Logic(renderer);
	//---------------------

	//Close
	SDL_DestroyRenderer(renderer);

	//Destroy window
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;

	//Quit SDL subsystems 
	SDL_Quit();

	return 0;
}

/*
{
	//Inicialización
	bool end = false;


		Bucle principal que hace cosas, con input y render

	while (!end)
	{
		end = true;
		//Input(); // No en esta práctica

		//Update();

		////Render
		//Renderer::Clear();
		//Renderer::PutPixel(); //--> Es estático
		//Renderer::Present();
	}

	return 0;

}
*/