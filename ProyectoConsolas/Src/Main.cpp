#include "Renderer/Renderer.h"
#include "Platform/Platform.h"

//#include "Logic/Fire.h"
//#include "Logic/Bars.h"
#include "Logic/Waves.h"

#include <stdlib.h>		/* srand, rand */
#include <time.h>       /* time */
#include <iostream>		/* cout */
#include <stdio.h>		/* fopen */

//Atributos de la ventana
const int NUM_BUFFERS = 1;	//PC : 1(Modo ventana) O 2(Modo fullscreen). PS4 2-16 //TODO: DETECTAR ERROR?
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;	//PS4: Tiene que tener relación de aspecto 16:9. Solo se tiene en cuenta el height
const Color SCREEN_CLEAR_COLOR = { 0,0,0,255 };

//Dimensiones fuego
//const int FIRE_HEIGHT = 100;
//const int FIRE_WIDTH = 480;
//
////Ciclo del fuego
//const int FIRE_TURN_ON_FRAMES = 300;
//const int FIRE_TURN_OFF_FRAMES = 150;
//
////Dimensiones barras
//const int BAR_HEIGHT = 10;
//const int BAR_WIDTH = 100;
//
////Distancia barras
//const int BAR_VERT_DIST = 20;
//const int BAR_HORI_DIST = 20;

//Waves
const int ENERGY_WAVE = 32; //	31/32
//Tiempo entre 1 gota y la siguiente
const int MIN_FRAMES_BETWEEN_WAVES = 50;
const int MAX_FRAMES_BETWEEN_WAVES = 150;

////Macro que convierte la representación en RGB de la clase color.
//#define RGB(r, g, b) { (uint8_t)r, (uint8_t)g, (uint8_t)b,255 }

//Color BarColor = RGB(0xFF, 0xFF, 0xFF);
//
//Color firePalette[] = {
//				 RGB(0x00,0x00,0x00),
//				 RGB(0x07,0x07,0x07),
//				 RGB(0x1F,0x07,0x07),
//				 RGB(0x2F,0x0F,0x07),
//				 RGB(0x47,0x0F,0x07),
//				 RGB(0x57,0x17,0x07),
//				 RGB(0x67,0x1F,0x07),
//				 RGB(0x77,0x1F,0x07),
//				 RGB(0x8F,0x27,0x07),
//				 RGB(0x9F,0x2F,0x07),
//				 RGB(0xAF,0x3F,0x07),
//				 RGB(0xBF,0x47,0x07),
//				 RGB(0xC7,0x47,0x07),
//				 RGB(0xDF,0x4F,0x07),
//				 RGB(0xDF,0x57,0x07),
//				 RGB(0xDF,0x57,0x07),
//				 RGB(0xD7,0x5F,0x07),
//				 RGB(0xD7,0x5F,0x07),
//				 RGB(0xD7,0x67,0x0F),
//				 RGB(0xCF,0x6F,0x0F),
//				 RGB(0xCF,0x77,0x0F),
//				 RGB(0xCF,0x7F,0x0F),
//				 RGB(0xCF,0x87,0x17),
//				 RGB(0xC7,0x87,0x17),
//				 RGB(0xC7,0x8F,0x17),
//				 RGB(0xC7,0x97,0x1F),
//				 RGB(0xBF,0x9F,0x1F),
//				 RGB(0xBF,0x9F,0x1F),
//				 RGB(0xBF,0xA7,0x27),
//				 RGB(0xBF,0xA7,0x27),
//				 RGB(0xBF,0xAF,0x2F),
//				 RGB(0xB7,0xAF,0x2F),
//				 RGB(0xB7,0xB7,0x2F),
//				 RGB(0xB7,0xB7,0x37),
//				 RGB(0xCF,0xCF,0x6F),
//				 RGB(0xDF,0xDF,0x9F),
//				 RGB(0xEF,0xEF,0xC7),
//				 RGB(0xFF,0xFF,0xFF)
//};

/*
	Aplicación que simula un fuego en la parte inferior de la pantalla junto con unas barras blancas que se desplazan a la derecha en la parte superior
	El fuego se enciende y se apaga ciclicamente
*/
int main(int argc, char* args[])
{
	//Inicialización
	Platform::Init(SCREEN_WIDTH, SCREEN_HEIGHT, NUM_BUFFERS);
	Renderer::Init();

	int numBuffers = Renderer::GetNumBuffers();

	////Inicializa la semilla de aleatoriedad random.
	srand(time(NULL));

	//La imagen de fondo es formato crudo
	Color **background = nullptr;

	//Inicialización fichero
	FILE *f;
	//f = fopen("app0/fdi.rgba", "rb");
	f = fopen("app0/fdi.rgba", "rb");	//PC: Projects/app0/..

	std::cout << "Holita" << std::endl;
	f = NULL;
	if (f != NULL)
	{
		std::cout << "Fichero abierto" << std::endl;
		//long lSize;

		//// obtain file size:
		//fseek(f, 0, SEEK_END);
		//lSize = ftell(f);
		//rewind(f);

		//char * buffer;

		//// allocate memory to contain the whole file:
		//buffer = (char*)malloc(sizeof(char)*lSize);
		//if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }

		//// copy the file into the buffer:
		//result = fread(buffer, 1, lSize, pFile);
		//if (result != lSize) { fputs("Reading error", stderr); exit(3); }

		fclose(f);
	}

	//No hay imagen, color por defecto
	else
	{
		background = new Color*[SCREEN_HEIGHT];
		for (int i = 0; i < SCREEN_HEIGHT; ++i)
		{
			background[i] = new Color[SCREEN_WIDTH];
			for (size_t j = 0; j < SCREEN_WIDTH; j++)
				background[i][j] = SCREEN_CLEAR_COLOR;
		}

	}

	////Inicialización fuego
	//Fire* fire = new Fire(FIRE_WIDTH, FIRE_HEIGHT, firePalette, sizeof(firePalette) / sizeof(Color));
	//int fireCycleFrames = FIRE_TURN_ON_FRAMES + FIRE_TURN_OFF_FRAMES;
	//bool fireOn = true;

	////Inicialización de las barras
	//Bars* bars = new Bars(BAR_WIDTH, BAR_HEIGHT, BAR_HORI_DIST, BAR_VERT_DIST, BarColor, SCREEN_CLEAR_COLOR, FIRE_HEIGHT);

	//Inicialización ondas
	Waves * waves = new Waves(ENERGY_WAVE, MIN_FRAMES_BETWEEN_WAVES, MAX_FRAMES_BETWEEN_WAVES, background);

	int tick = 0;
	Renderer::Clear(SCREEN_CLEAR_COLOR); //TODO: En esta práctica no hacemos clear para mantener la coherencia de frames de las barras

	//Bucle principal
	while (Platform::Tick())
	{
		//Input(); // TODO: No en esta práctica


		////Actualización fuego
		//fireOn = ((tick % fireCycleFrames) < FIRE_TURN_ON_FRAMES);	//Detecta si está apagado o encendido
		//fire->Update(fireOn);

		////Dibujado
		//fire->Render();

		////Las barras cada frame se mueven 1 unidad
		////Si no están inicializados todos los buffers. Se dibujan las barras en 'tick' position
		//if (tick < numBuffers)
		//	bars->Draw(tick);

		////Si están inicializados, se dibujan en función del numero de Buffers
		//else
		//	bars->DrawWithDelta(tick, numBuffers);

		waves->Update(tick);
		waves->Render();

		Renderer::Present();

		SDL_Delay(0);

		tick++;
	}

	//delete bars;
	//bars = nullptr;
	//delete fire;
	//fire = nullptr;

	for (int i = 0; i < SCREEN_HEIGHT; ++i)
		delete[] background[i];

	delete[] background;

	Renderer::Release();
	Platform::Release();

	return 0;
}
