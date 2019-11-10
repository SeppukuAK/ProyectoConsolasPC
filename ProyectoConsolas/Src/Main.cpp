#include "Renderer/Renderer.h"
#include "Platform/Platform.h"

#include "Logic/Fire.h"
#include "Logic/Bars.h"

#include <stdlib.h>		/* srand, rand */
#include <time.h>       /* time */

//Atributos de la ventana
const int NUM_BUFFERS = 2;	//PC : 1(Modo ventana) O 2(Modo fullscreen). PS4 1-16 //TODO: DETECTAR ERROR?	
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Dimensiones fuego
const int FIRE_HEIGHT = 100;
const int FIRE_WIDTH = 480;

//Ciclo del fuego
const int FIRE_TURN_ON_FRAMES = 300;
const int FIRE_TURN_OFF_FRAMES = 150;

//Dimensiones barras
const int BAR_HEIGHT = 10;
const int BAR_WIDTH = 100;

//Distancia barras
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

	//Inicializa la semilla de aleatoriedad random.
	srand(time(NULL));

	//Inicialización fuego
	Fire* fire = new Fire(FIRE_WIDTH, FIRE_HEIGHT, firePalette, sizeof(firePalette) / sizeof(Color));
	int fireCycleFrames = FIRE_TURN_ON_FRAMES + FIRE_TURN_OFF_FRAMES;
	bool fireOn = true;

	//Inicialización de las barras
	Bars* bars = new Bars(BAR_WIDTH, BAR_HEIGHT, BAR_HORI_DIST, BAR_VERT_DIST, RGB(255, 255, 255), RGB(0, 0, 0), FIRE_HEIGHT);

	int tick = 0;

	//Bucle principal
	while (Platform::Tick())
	{
		//Input(); // TODO: No en esta práctica

		//Renderer::Clear(); //TODO: En esta práctica no hacemos clear para mantener la coherencia de frames de las barras

		//Actualización fuego
		fireOn = ((tick % fireCycleFrames) < FIRE_TURN_ON_FRAMES);	//Detecta si está apagado o encendido
		fire->Update(fireOn);

		//Dibujado
		fire->Render();

		//Las barras cada frame se mueven 1 unidad
		//Si no están inicializados todos los buffers. Se dibujan las barras en 'tick' position
		if (tick < numBuffers)
			bars->Draw(tick);

		//Si están inicializados, se dibujan en función del numero de Buffers
		else
			bars->DrawWithDelta(tick, numBuffers);

		Renderer::Present();

		//Espera 'x' milisegundos
		//SDL_Delay(50); //TODO: ESTO NO TIENE QUE SER ASI OBVIAMENTE

		tick++;
	}

	//TODO: Los null de Adri
	delete bars;
	bars = nullptr;
	delete fire;
	fire = nullptr;

	Renderer::Release();
	Platform::Release();

	return 0;
}
