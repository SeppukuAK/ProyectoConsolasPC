//Se envuelve todo para evitar que se compile si no estamos en la plataforma
#if PLATFORM_PC

#include "PlatformPC.h"
#include <stdio.h>		/* fopen */

//Inicialización de atributos estáticos
SDL_Window* PlatformPC::window = NULL;
std::string PlatformPC::mediaPath = "../Media/";//Se accede desde Projects

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
		//Se cierra la ventana si se da a la 'X' o cualquier tecla o el ratón
		if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
			quit = true;
	}

	//Si quiere salir devuelve false
	return !quit;
}

Image* PlatformPC::LoadImage(std::string path)
{
	Color* arrayColor = nullptr;
	int width = 0;
	int height = 0;

	//Inicialización fichero
	FILE* f = NULL;

	f = fopen((mediaPath + path).c_str(), "rb");

	//Imagen cargada
	if (f != NULL)
	{
		uint8_t buffer[4];

		fread(buffer, sizeof(int), 1, f);
		width = (buffer[3] << 0) | (buffer[2] << 8) | (buffer[1] << 16) | (buffer[0] << 24);

		fread(buffer, sizeof(int), 1, f);
		height = (buffer[3] << 0) | (buffer[2] << 8) | (buffer[1] << 16) | (buffer[0] << 24);

		arrayColor = new Color[width * height];	//Se asume que el fichero tiene la misma resolución que la ventana

		// fucntion used to read the contents of file 
		fread(arrayColor, sizeof(Color), width * height, f);

		fclose(f);
	}
	else
	{
		printf("Error al cargar la imagen");
	}

	Image * image = new Image(arrayColor, width, height);

	return image;
}

#endif
