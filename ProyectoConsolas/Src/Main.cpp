#include "Platform/Platform.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererThread.h"
#include "Input/Input.h"
#include "Logic//WestBank.h"

#include <stdlib.h>		/* srand, rand */
#include <time.h>       /* time */
#include <chrono>		/* chrono */
#include <iostream>		/* cout */		

//Utilizado para el deltaTime
using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(16ms);	// we use a fixed timestep of 1 / (60 fps) = 16 milliseconds

using namespace std;

//Atributos de la ventana
const int NUM_BUFFERS = 1;							//PC : 1(Modo ventana) O 2(Modo fullscreen). PS4 2-16 //TODO: DETECTAR ERROR?
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;						//PS4: Tiene que tener relación de aspecto 16:9. Solo se tiene en cuenta el height
const Color SCREEN_CLEAR_COLOR = { 0,0,0,255 };

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

	//Creación de comandos iniciales
	RenderCommand clearCommand;
	clearCommand.Type = RendererCommandType::CLEAR;
	clearCommand.Param.ClearParams.Color = SCREEN_CLEAR_COLOR;

	RenderCommand presentCommand;
	presentCommand.Type = RendererCommandType::END_FRAME;

	//Limpieza inicial de los buffers
	for (int i = 0; i < NUM_BUFFERS; i++)
	{
		RendererThread::EnqueueCommand(clearCommand);
		RendererThread::EnqueueCommand(presentCommand);
	}

	//Inicialización juego
	WestBank::Init();

	//Contención inicial. No empieza el bucle principal hasta que se limpien todos los buffers
	while (RendererThread::GetPendingFrames() > 0)
		;

	//Variables bucle
	int tick = 0;
	float time = 0.0f;

	float deltaTime = timestep.count() / 1000000000.0f;//Tiempo entre frames

	using clock = std::chrono::high_resolution_clock;

	std::chrono::nanoseconds lag(0ns);
	auto time_start = clock::now();

	//Bucle principal. Hebra lógica.
	while (Platform::Tick())
	{
		auto delta_time = clock::now() - time_start;
		time_start = clock::now();
		lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

		Input::Tick();

		WestBank::Input();

		// update game logic as lag permits
		while (lag >= timestep) 
		{
			time += deltaTime;

			WestBank::Update(time,tick);

			//waves->Update();

			lag -= timestep;
		}

		//Contención. Se para la hebra si el render va muy retrasado
		while (RendererThread::GetPendingFrames() >= NUM_BUFFERS)
			;

		//RendererThread::EnqueueCommand(clearCommand); // En esta práctica no se hace clear para mantener la coherencia de frames

		//Comunicación logica con render: se le envia a la hebra de render el comando con las nuevas condiciones de la logica.
		WestBank::Render();

		RendererThread::EnqueueCommand(presentCommand);

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
