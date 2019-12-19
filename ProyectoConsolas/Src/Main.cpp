#include "Platform/Platform.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererThread.h"
#include "Input/Input.h"
#include "Utilities/Time.h"
#include "Logic/WestBank.h"

#include <stdlib.h>		/* srand, rand */
#include <iostream>		/* cout */		

using namespace std;

//Atributos de la ventana
const int NUM_BUFFERS = 1;							//PC : 1(Modo ventana) O 2(Modo fullscreen). PS4 2-16 //TODO: DETECTAR ERROR?
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;						//PS4: Tiene que tener relación de aspecto 16:9. Solo se tiene en cuenta el height

const double MS_PER_UPDATE = 1000.0 / 60;			//FPS a los que trata de ir

/*
	WestBank
	Multihebra: 1 Lógica y 1 Render -> Cada una va a un nucleo
*/
int main(int argc, char* args[])
{
	//Inicializacion de la semilla de aleatoriedad.
	srand(time(NULL));

	//Inicialización motor
	Platform::Init();
	Renderer::Init(SCREEN_WIDTH, SCREEN_HEIGHT, NUM_BUFFERS);
	Input::Init();

	//Se lanza la hebra de renderizado
	RendererThread::Start();

	//Inicialización juego
	WestBank::Init();

	//Contención inicial. No empieza el bucle principal hasta que se limpien todos los buffers
	while (RendererThread::GetPendingFrames() > 0)
		;

	//Inicialización time
	Time::Init();

	//Variables bucle
	int tick = 0;
	double lag = 0.0;

	//Bucle principal. Hebra lógica.
	while (Platform::Tick())
	{
		//Actualización time
		Time::Tick();
		lag += Time::deltaTime * 1000;

		//Detección input
		Input::Tick();

		//Input juego
		WestBank::Input();

		while (lag >= MS_PER_UPDATE)
		{
			//Update juego
			WestBank::Update(tick);
			lag -= MS_PER_UPDATE;
		}

		//Contención. Se para la hebra si el render va muy retrasado
		while (RendererThread::GetPendingFrames() >= NUM_BUFFERS)
			;

		//RendererThread::EnqueueClearCommand(); // En esta práctica no se hace clear para mantener la coherencia de frames

		//Comunicación logica con render: se le envia a la hebra de render el comando con las nuevas condiciones de la logica.
		WestBank::Render();

		//Se pinta el estado actual
		RendererThread::EnqueuePresentCommand();

		tick++;
	}

	//Para la hebra de renderizado
	RendererThread::Stop();

	//Destruye el juego
	WestBank::Release();

	//Destrucción motor
	Input::Release();
	Renderer::Release();
	Platform::Release();

	return 0;
}
