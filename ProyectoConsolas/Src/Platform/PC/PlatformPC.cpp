//Se envuelve todo para evitar que se compile si no estamos en la plataforma
#if PLATFORM_PC

#include "PlatformPC.h"
#include <iostream>		/* cout */
#include <stdio.h>		/* fopen */
#include <SDL.h>		/* SDL. Pintado */
#include "../../Input/InputObserver.h"
#include "../../Renderer/Image.h"
#include "../../Renderer/Color.h"

using namespace std;

//Inicialización de atributos estáticos
const string PlatformPC::mediaPath = "../Media/";	//Se accede desde Projects

SDL_Window* PlatformPC::window = NULL;
vector<InputObserver*> PlatformPC::observers = vector<InputObserver*>();

void PlatformPC::Init(int screenWidth, int screenHeight, int numBuffers)
{
	//La ventana en la que se renderizará
	window = NULL;

	//Inicialización de SDL y GameController
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
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

	/*
		Procesa los eventos de la cola:
		-Se cierra la ventana si se da a la 'X' o a la tecla escape o el ratón.
		-Cualquier otro evento informa a los observers
	*/
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
		case SDL_MOUSEBUTTONDOWN:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_ESCAPE)
				quit = true;
			else
				NotifyObservers(e);
			break;
		default:
			NotifyObservers(e);
			break;
		}
	}

	//Si quiere salir devuelve false
	return !quit;
}

Image* PlatformPC::LoadImage(string path)
{
	Image* image = nullptr;
	Color* arrayColor = nullptr;
	int width = 0;
	int height = 0;

	//Inicialización fichero
	FILE* f = NULL;

	f = fopen((mediaPath + path).c_str(), "rb");

	//Imagen cargada
	if (f != NULL)
	{
		//Lectura de cabecera: 2 enteros de 4 bytes (big endian)
		uint8_t buffer[4];

		//TODO: Meter en método?
		fread(buffer, sizeof(int), 1, f);
		width = (buffer[3] << 0) | (buffer[2] << 8) | (buffer[1] << 16) | (buffer[0] << 24);

		fread(buffer, sizeof(int), 1, f);
		height = (buffer[3] << 0) | (buffer[2] << 8) | (buffer[1] << 16) | (buffer[0] << 24);

		//Lectura de la imagen
		int imageSize = width * height;
		arrayColor = new Color[imageSize];

		fread(arrayColor, sizeof(Color), imageSize, f);

		fclose(f);

		image = new Image(arrayColor, width, height);
	}
	else
	{
		printf("Error al cargar la imagen");
	}

	return image;
}

void PlatformPC::RegisterObserver(InputObserver* observer)
{
	observers.push_back(observer);
}

void PlatformPC::RemoveObserver(InputObserver* observer)
{
	// find the observer
	auto iterator = find(observers.begin(), observers.end(), observer);

	if (iterator != observers.end()) { // observer found
		observers.erase(iterator); // remove the observer
	}
}

void PlatformPC::NotifyObservers(SDL_Event e)
{
	bool eventConsumed = false;
	int i = 0;

	//Notifica a todos los observers el evento hasta que uno lo consuma
	while (!eventConsumed && i < observers.size())
	{
		eventConsumed = observers[i]->HandleEvent(e);
		i++;
	}
}

#endif
