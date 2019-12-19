#include "Platform/Platform.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererThread.h"
#include "Input/Input.h"
#include "Logic//WestBank.h"

#include <stdlib.h>		/* srand, rand */
#include <time.h>       /* time */
#include <iostream>		/* cout */		

#include <chrono>
#include <ctime>    
#include <thread>		/* sleep_for */

using namespace std;

//Atributos de la ventana
const int NUM_BUFFERS = 1;							//PC : 1(Modo ventana) O 2(Modo fullscreen). PS4 2-16 //TODO: DETECTAR ERROR?
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;						//PS4: Tiene que tener relación de aspecto 16:9. Solo se tiene en cuenta el height

const double MS_PER_UPDATE = 1000.0 / 60;

////Waves
////const float ENERGY_WAVE = 31/ 32;
//const int HEIGHT_WAVE = 5000;
//
////Tiempo minimo y maximo entre 1 gota y la siguiente
//const int MIN_FRAMES_BETWEEN_WAVES = 50;
//const int MAX_FRAMES_BETWEEN_WAVES = 150;


/*
	Aplicación que muestra un efecto de lluvia que cae sobre una imagen de la facultad.
	Multihebra: 1 Lógica y 1 Render -> Cada una va a un nucleo
*/
int main(int argc, char* args[])
{
	//Inicializa la semilla de aleatoriedad random.
	srand(time(NULL));

	//Inicialización
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

	//Variables bucle
	int tick = 0;

	auto startGameTime = std::chrono::system_clock::now();
	double previous = 0.0;
	double lag = 0.0;

	//Bucle principal. Hebra lógica.
	while (Platform::Tick())
	{
		//GetCurrentTime
		auto startFrameTime = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = startFrameTime - startGameTime;
		double current = elapsed_seconds.count();

		double elapsed = current - previous;
		previous = current;
		lag += elapsed * 1000;

		Input::Tick();
		WestBank::Input();

		while (lag >= MS_PER_UPDATE)
		{
			WestBank::Update(current, tick);
			//waves->Update();
			lag -= MS_PER_UPDATE;
		}

		//Contención. Se para la hebra si el render va muy retrasado
		while (RendererThread::GetPendingFrames() >= NUM_BUFFERS)
			;

		//RendererThread::EnqueueClearCommand(); // En esta práctica no se hace clear para mantener la coherencia de frames

		//Comunicación logica con render: se le envia a la hebra de render el comando con las nuevas condiciones de la logica.
		WestBank::Render();

		RendererThread::EnqueuePresentCommand();

		//Delay
		tick++;
	}

	RendererThread::Stop();

	WestBank::Release();

	Input::Release();
	Renderer::Release();
	Platform::Release();

	return 0;
}
