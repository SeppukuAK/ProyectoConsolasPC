//Se envuelve todo para evitar que se compile si no estamos en la plataforma
#if PLATFORM_PC

#include "PlatformPC.h"
#include <stdio.h> 

//Inicialización de atributos estáticos
SDL_Window* PlatformPC::window = NULL;

void PlatformPC::Init(int screenWidth, int screenHeight, int numBuffers)
{
	//La ventana en la que se renderizará
	window = NULL;

	//Inicialización de SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

	//Si no ha habido error
	else
	{
		//Crea la ventana en función del modo establecido. Fullscreen 2 buffer. Windowed 1 buffer.
		if (numBuffers == 2)
			window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
		else if (numBuffers == 1)
			window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		else
			printf("Error: En PC solo se dispone de 1 o 2 RenderBuffer");

		if (window == NULL)
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());

	}
}

void PlatformPC::Release()
{
	//Destrucción de la ventana
	SDL_DestroyWindow(window);
	window = NULL;

	//Cierra SDL
	SDL_Quit();
}

bool PlatformPC::Tick()
{
	SDL_Event e;		//Manejador de eventos
	bool quit = false;

	//Procesa los eventos de la cola
	while (SDL_PollEvent(&e) != 0)
	{
		//Se cierra la ventana si se da a la 'X' o a cualquier tecla
		if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
			quit = true;
	}

	//Si quiere salir devuelve false
	return !quit;
}

#endif
