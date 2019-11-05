//#include "Renderer/Renderer.h"
//#include "Platform/Platform.h"
#include "Renderer/Utilities/Color.h"

//#include
//#include <iostream>

//Using SDL and standard IO 
#include <SDL.h>
#include <stdio.h> 
#include <iostream>
#include <time.h>       /* time */
#include <stdlib.h>		/* srand, rand */

using namespace std;

//Screen dimension constants 
const bool FULLSCREEN = false;	//Fullscreen tiene doble buffer, no fullscreen un único buffer
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Dimensiones fuego
const int FIRE_HEIGHT = 100;
const int FIRE_WIDTH = 480;

//Dimensiones barras
const int BAR_HEIGHT = 10;
const int BAR_WIDTH = 100;

const int BAR_VERT_DIST = 20;
const int BAR_HORI_DIST = 20;

//Macro que convierte la representación en RGB de la clase color.
#define RGB(r, g, b) { (uint8_t)r, (uint8_t)g, (uint8_t)b,255 }

Color firePalette[] = {
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

void FireStep(SDL_Renderer* renderer, int** fireMatrix, int firePosX, int firePosY)
{
	//Fuego
	for (int i = (FIRE_HEIGHT - 2); i >= 0; i--)   		//Recorremos de abajo a arriba
	{
		for (int j = 0; j < FIRE_WIDTH; j++)
		{
			//Evolucionamos cada pixel, actualizando su temperatura

			//Elegimos de forma aleatoria que dirección escoge
			int randomDir; //Coge un valor entre [-1,+1]

			//Ajustamos casos de error
			if (j == 0)
				randomDir = rand() % 2;
			else if (j == FIRE_WIDTH - 1)
				randomDir = rand() % 2 - 1;
			else
				randomDir = rand() % 3 - 1;

			//Elegimos de forma aleatoria si se enfria o no
			int randomCooling = rand() % 2; //Coge un valor entre [0,+1]

			fireMatrix[i][j] = fireMatrix[i + 1][j + randomDir] - randomCooling;

			if (fireMatrix[i][j] < 0)
				fireMatrix[i][j] = 0;

			Color newColor = firePalette[fireMatrix[i][j]];

			//Esto es PutPixel
			SDL_SetRenderDrawColor(renderer, newColor.R, newColor.G, newColor.B, newColor.A);

			//Cambiamos el color del pixel
			SDL_RenderDrawPoint(renderer, j + firePosX, i + firePosY);
		}
	}
}

void Logic(SDL_Renderer* renderer, int numBuffers)
{


	//Clear screen
	SDL_RenderClear(renderer);

	//Inicialización atributos del fuego

	//Obtenemos numero de colores de la paleta del fuego
	int sizeFirePalette = sizeof(firePalette) / sizeof(Color);

	//Cada valor representa la temperatura del fuego. Valores entre 0(frio) y sizeFirePalette(caliente):
	int** fireMatrix = NULL;

	fireMatrix = new int* [FIRE_HEIGHT];
	for (int i = 0; i < FIRE_HEIGHT; ++i)
		fireMatrix[i] = new int[FIRE_WIDTH];

	Color initialColor = firePalette[sizeFirePalette - 1]; 	//Color inicial (blanco)
	int firePosY = SCREEN_HEIGHT - FIRE_HEIGHT;
	int firePosX = SCREEN_WIDTH / 2 - FIRE_WIDTH / 2;

	//Inicialización atributos de Barras
	int numHoriBars = SCREEN_WIDTH / (BAR_WIDTH + BAR_HORI_DIST);
	int numVertBars = (SCREEN_HEIGHT - FIRE_HEIGHT) / (BAR_HEIGHT + BAR_VERT_DIST);
	Color barColor = RGB(255, 255, 255);		//Las barras son de color blanco

	//Hay que inicializar los frame Buffers disponibles

	SDL_SetRenderDrawColor(renderer, initialColor.R, initialColor.G, initialColor.B, initialColor.A);

	//Ponemos la primera fila a la temperatura más caliente
	for (int j = 0; j < FIRE_WIDTH; j++)
	{
		fireMatrix[FIRE_HEIGHT - 1][j] = sizeFirePalette - 1;
		SDL_RenderDrawPoint(renderer, j + firePosX, (FIRE_HEIGHT - 1) + firePosY);
	}

	SDL_SetRenderDrawColor(renderer, barColor.R, barColor.G, barColor.B, barColor.A);

	//Pintamos las barras del primer buffer
	for (int r = 0; r < numVertBars; r++)
		for (int c = 0; c < numHoriBars; c++)
			for (int i = 0; i < BAR_HEIGHT; i++)
				for (int j = 0; j < BAR_WIDTH; j++)
					SDL_RenderDrawPoint(renderer, j + c * (BAR_WIDTH + BAR_HORI_DIST), i + r * (BAR_HEIGHT + BAR_VERT_DIST));

	//Update screen
	SDL_RenderPresent(renderer);

	//Pintamos todos los frames buffers siguientes
	for (int b = 1; b < numBuffers; b++)
	{
		SDL_SetRenderDrawColor(renderer, initialColor.R, initialColor.G, initialColor.B, initialColor.A);

		//Actualizamos el color de la primera fila
		for (int j = 0; j < FIRE_WIDTH; j++)
			SDL_RenderDrawPoint(renderer, j + firePosX, (FIRE_HEIGHT - 1) + firePosY);

		//Avanzamos un tick la simulación del fuego
		FireStep(renderer, fireMatrix, firePosX, firePosY);

		SDL_SetRenderDrawColor(renderer, barColor.R, barColor.G, barColor.B, barColor.A);

		//Pintamos las barras del frame buffer: b
		for (int r = 0; r < numVertBars; r++)
			for (int c = 0; c < numHoriBars; c++)
				for (int i = 0; i < BAR_HEIGHT; i++)
					for (int j = 0; j < BAR_WIDTH; j++)
						SDL_RenderDrawPoint(renderer, j + c * (BAR_WIDTH + BAR_HORI_DIST) + b, i + r * (BAR_HEIGHT + BAR_VERT_DIST));

		//Update screen
		SDL_RenderPresent(renderer);
	}

	//Simulación
	int totalTicks = 1000;

	/* initialize random seed: */
	srand(time(NULL));

	//Event handler
	SDL_Event e; //TODO: VA EN PLATFORM
	//Main loop flag
	bool quit = false;

	for (int delta = numBuffers; (delta < totalTicks) && !quit; delta++)
	{
		//------------input PC-------------------
		 //Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		//------------input PC-------------------

		//En cada frame evolucionamos la simulacion
		FireStep(renderer, fireMatrix, firePosX, firePosY);

		//Barras
		//Cada frame se mueven una unidad. 
		/*Para aumentar velocidad, aprovechamos la coherencia entre frames(no hay que utilizar putPixel todo el rato),
		solo se borra la columna izquierda de cada barra y se pinta la columna derecha*/

		//Pintar cada fila
		for (int r = 0; r < numVertBars; r++)
		{
			//Pintar cada columna
			for (int c = 0; c < numHoriBars; c++)
			{
				Color black = RGB(0, 0, 0);
				SDL_SetRenderDrawColor(renderer, black.R, black.G, black.B, black.A);

				//Borramos las columnas de la izquierda antiguas del buffer
				for (int b = 0; b < numBuffers; b++)
					for (int i = 0; i < BAR_HEIGHT; i++)
						SDL_RenderDrawPoint(renderer, (delta - (b + 1) + c * (BAR_WIDTH + BAR_HORI_DIST)) % SCREEN_WIDTH, i + r * (BAR_HEIGHT + BAR_VERT_DIST));

				//Pintamos las columnas del nuevo buffer por la derecha
				Color white = RGB(255, 255, 255);
				SDL_SetRenderDrawColor(renderer, white.R, white.G, white.B, white.A);

				for (int b = 0; b < numBuffers; b++)
					for (int i = 0; i < BAR_HEIGHT; i++)
						SDL_RenderDrawPoint(renderer, (delta + BAR_WIDTH - (b + 1) + c * (BAR_WIDTH + BAR_HORI_DIST)) % SCREEN_WIDTH, i + r * (BAR_HEIGHT + BAR_VERT_DIST));
			}
		}

		//Update screen
		SDL_RenderPresent(renderer);

		SDL_Delay(50);
	}

	//Limpiamos memoria
	for (int i = 0; i < FIRE_HEIGHT; ++i)
		delete[] fireMatrix[i];

	delete[] fireMatrix;
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
		//Crea la ventana en función del modo establecido. Fullscreen 2 buffer. Windowed 1 buffer.
		if (FULLSCREEN)
			window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
		else
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
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
			}

		}
	}


	//---------------------
	if (FULLSCREEN)
		Logic(renderer, 2);
	else
		Logic(renderer, 1);

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
		//Renderer::Clear(); //En esta práctica no hacemos clear() por las barras. Para mantener la coherencia de frames
		//Renderer::PutPixel(); //--> Es estático
		//Renderer::Present();
	}

	return 0;

}
*/