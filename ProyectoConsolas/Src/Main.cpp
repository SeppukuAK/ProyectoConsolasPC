#include "Platform/Platform.h"
#include "Renderer/RendererThread.h"

#include "Logic/Waves.h"

#include <stdlib.h>		/* srand, rand */
#include <time.h>       /* time */
#include <stdio.h>		/* fopen */
#include <iostream>		/* cout */		

using namespace std;

//Atributos de la ventana
const int NUM_BUFFERS = 1;							//PC : 1(Modo ventana) O 2(Modo fullscreen). PS4 2-16 //TODO: DETECTAR ERROR?
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;						//PS4: Tiene que tener relaci�n de aspecto 16:9. Solo se tiene en cuenta el height
const Color SCREEN_CLEAR_COLOR = { 0,0,0,255 };

//Waves
//const float ENERGY_WAVE = 31/ 32;
const int HEIGHT_WAVE = 5000;

//Tiempo minimo y maximo entre 1 gota y la siguiente
const int MIN_FRAMES_BETWEEN_WAVES = 50;
const int MAX_FRAMES_BETWEEN_WAVES = 150;

/*
	Aplicaci�n que muestra un efecto de lluvia que cae sobre una imagen de la facultad.
	Multihebra: 1 L�gica y 1 Render -> Cada una va a un nucleo
*/
int main(int argc, char* args[])
{
	//Inicializaci�n
	Platform::Init(SCREEN_WIDTH, SCREEN_HEIGHT, NUM_BUFFERS);

	//Inicializa la semilla de aleatoriedad random.
	srand(time(NULL));

	//La imagen de fondo es formato crudo
	Image * image = Platform::LoadImage("SpritesSheetsWestBank/ladron.rgba");

	//Se lanza la hebra de renderizado
	RendererThread* rendererThread = new RendererThread(); // TODO: NO HACER NEW PARA CREAR LA CLASE RENDERTRHEAD
	rendererThread->Start();

	//Inicializaci�n ondas
	//Waves* waves = new Waves(background, HEIGHT_WAVE, MIN_FRAMES_BETWEEN_WAVES, MAX_FRAMES_BETWEEN_WAVES);

	//Creaci�n de comandos iniciales
	RenderCommand clearCommand;
	clearCommand.Type = RendererCommandType::CLEAR;
	clearCommand.Param.ClearParams.Color = SCREEN_CLEAR_COLOR;

	RenderCommand presentCommand;
	presentCommand.Type = RendererCommandType::END_FRAME;

	//Limpieza inicial de los buffers
	for (int i = 0; i < NUM_BUFFERS; i++)
	{
		rendererThread->EnqueueCommand(clearCommand);
		rendererThread->EnqueueCommand(presentCommand);
	}

	//Contenci�n inicial. No empieza el bucle principal hasta que se limpien todos los buffers
	while (rendererThread->GetPendingFrames() > 0)
		;

	int tick = 0;

	//Bucle principal. Hebra l�gica.
	while (Platform::Tick())
	{
		//Input(); // No en esta pr�ctica
		//rendererThread->EnqueueCommand(clearCommand); // En esta pr�ctica no se hace clear para mantener la coherencia de frames

		//Paso de simulaci�n
		//waves->Update(tick);

		//Contenci�n. Se para la hebra si el render va muy retrasado
		while (rendererThread->GetPendingFrames() >= NUM_BUFFERS)
			;

		//Comunicaci�n logica con render: se le envia a la hebra de render el comando con las nuevas condiciones de la logica.
		//rendererThread->EnqueueCommand(waves->GetRenderCommand());

		for (int i = 0; i < image->GetHeight(); i++)
		{
			for (int j = 0; j < image->GetWidth(); j++)
			{
				RenderCommand pixelCommand;
				pixelCommand.Type = RendererCommandType::PUT_PIXEL;
				pixelCommand.Param.PutPixelParams.Color = image->GetColorArray()[i * image->GetWidth() + j];
				pixelCommand.Param.PutPixelParams.X = j;
				pixelCommand.Param.PutPixelParams.Y = i;
				rendererThread->EnqueueCommand(pixelCommand);
			}
		}

		rendererThread->EnqueueCommand(presentCommand);

		//Delay
		tick++;
	}

	rendererThread->Stop();

	//delete waves;
	//waves = nullptr;

	delete rendererThread;
	rendererThread = nullptr;

	delete image;
	image = nullptr;

	Platform::Release();

	return 0;
}
