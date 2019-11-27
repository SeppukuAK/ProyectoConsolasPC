#include "Platform/Platform.h"
#include "Renderer/RendererThread.h"

#include "Logic/Waves.h"

#include <stdlib.h>		/* srand, rand */
#include <time.h>       /* time */
#include <iostream>		/* cout */
#include <stdio.h>		/* fopen */

using namespace std;

//Atributos de la ventana
const int NUM_BUFFERS = 2;	//PC : 1(Modo ventana) O 2(Modo fullscreen). PS4 2-16 //TODO: DETECTAR ERROR?
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;	//PS4: Tiene que tener relación de aspecto 16:9. Solo se tiene en cuenta el height
const Color SCREEN_CLEAR_COLOR = { 0,0,0,255 };

//Waves
//const float ENERGY_WAVE = 31/ 32; //TODO: Hay que utilizarla?
const int HEIGHT_WAVE = 5000;

//Tiempo minimo y maximo entre 1 gota y la siguiente
const int MIN_FRAMES_BETWEEN_WAVES = 50;
const int MAX_FRAMES_BETWEEN_WAVES = 150;

/*
	TODO: PONER COMENTARIO
	Aplicación que simula un fuego en la parte inferior de la pantalla junto con unas barras blancas que se desplazan a la derecha en la parte superior
	El fuego se enciende y se apaga ciclicamente
*/
int main(int argc, char* args[])
{
	//Inicialización
	Platform::Init(SCREEN_WIDTH, SCREEN_HEIGHT, NUM_BUFFERS);

	//Inicializa la semilla de aleatoriedad random.
	srand(time(NULL));

	//La imagen de fondo es formato crudo
	Color* background = new Color[SCREEN_HEIGHT * SCREEN_WIDTH];	//Se asume que el fichero tiene la misma resolución que la ventana

	//Inicialización fichero
	FILE* f = NULL;

	f = fopen("app0/fdi.rgba", "rb");	//PC: Projects/app0/..
	
	//Imagen cargada
	if (f != NULL)
	{
		std::cout << "Fichero abierto" << std::endl;

		// fucntion used to read the contents of file 
		fread(background, sizeof(Color), SCREEN_HEIGHT * SCREEN_WIDTH, f);

		fclose(f);
	}

	//No hay imagen, color por defecto
	else
	{
		std::cout << "Error de apertura de fichero" << std::endl;

		for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++)
			background[i] = SCREEN_CLEAR_COLOR;
	}

	//Se lanza la hebra de renderizado
	RendererThread* rendererThread = new RendererThread();
	rendererThread->Start();

	//Inicialización ondas
	Waves* waves = new Waves(background, HEIGHT_WAVE, MIN_FRAMES_BETWEEN_WAVES, MAX_FRAMES_BETWEEN_WAVES);

	int tick = 0;

	//Creación de comandos iniciales
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

	//Bucle principal
	while (Platform::Tick())
	{
		//Input(); // No en esta práctica
		//rendererThread->EnqueueCommand(clearCommand); // En esta práctica no hacemos clear para mantener la coherencia de frames

		//Lógica. 1. Paso de simulación de la lluvia
		waves->Update(tick);

		//3. Contención. Se para la hebra si el render va muy retrasado
		while (rendererThread->GetPendingFrames() >= NUM_BUFFERS)
			;

		//Comunicación logica con render
		//2. Una vez terminada la simulacion, se le envia a la hebra de render el comando con las nuevas condiciones de la logica.
		rendererThread->EnqueueCommand(waves->GetRenderCommand());

		rendererThread->EnqueueCommand(presentCommand);

		//Delay

		tick++;
	}

	rendererThread->Stop();

	delete rendererThread;
	rendererThread = nullptr;

	delete[] background;
	background = nullptr;

	Platform::Release();

	return 0;
}
