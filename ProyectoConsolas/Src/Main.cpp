#include "Platform/Platform.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererThread.h"
#include "Input/Input.h"

//#include "Logic/Waves.h"
#include "Logic/Door.h"
#include "Logic/FrameDoor.h"
#include "Logic/Dollar.h"
#include "Logic/Client.h"

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

//Lógica

//Game
const float MIN_SECONDS_OPENING_DOOR = 4.0f;
const float MAX_SECONDS_OPENING_DOOR = 10.0f;
const float DOOR_OPENED_TIME = 2.0f;	//Incluye el tiempo abriendose

const int NUM_DOORS = 9;
const int NUM_VISIBLE_DOORS = 3;

const int GAME_X_OFFSET = 32;

//FrameDoor
const int FRAME_DOOR_WIDTH = 192;
const int FRAME_DOOR_OFFSETY = 48;

//Door
const int DOOR_ANIM_FRAMES = 4;
const int DOOR_OFFSET_Y = 24;

//Dollar
const int DOLLAR_WIDTH = 64;

static float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

////Waves
////const float ENERGY_WAVE = 31/ 32;
//const int HEIGHT_WAVE = 5000;
//
////Tiempo minimo y maximo entre 1 gota y la siguiente
//const int MIN_FRAMES_BETWEEN_WAVES = 50;
//const int MAX_FRAMES_BETWEEN_WAVES = 150;

enum ImageType { DOOR_FRAME, DOORS, CLIENT, THIEF, DOLLARS, BANG, SIZE };

void LoadResources(Image** images)
{
	//TODO: Esto esta bien o un array de string o qué?

	//Pared con puerta. //TODO: 3 De ellas consecutivas crean la escena principal de juego
	images[DOOR_FRAME] = Platform::LoadImage("SpritesSheetsWestBank/marcoPuerta.rgba");

	//Contiene las cuatro posibles posiciones de una puerta. Todos los sprites tienen el mismo ancho.
	//La zona abierta no debería pintarse, sino sustituirse por el personaje que abre la puerta
	images[DOORS] = Platform::LoadImage("SpritesSheetsWestBank/puertas.rgba");

	//Distintas "poses" de ambos personajes
	//Tienen el mismo ancho que el ancho que deja una puerta completamente abierta
	images[CLIENT] = Platform::LoadImage("SpritesSheetsWestBank/client.rgba");
	images[THIEF] = Platform::LoadImage("SpritesSheetsWestBank/ladron.rgba");

	//Distintos "sprites" de las "cajas" que indican el estado de una puerta. (Visible o No) (Recibe ingreso o no)
	images[DOLLARS] = Platform::LoadImage("SpritesSheetsWestBank/dolares.rgba");

	//Distintos "frames" de la animación de "Bang" mostrada cuando se recibe un disparo de un ladrón
	images[BANG] = Platform::LoadImage("SpritesSheetsWestBank/bang.rgba");
}

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

	//Carga de recursos. Asumen una resolución de juego de 640 x 360
	Image* images[ImageType::SIZE];
	LoadResources(images);

	//Inicialización lógica

	//Puertas
	//Inicialización
	FrameDoor::Init(images[ImageType::DOOR_FRAME]);
	Door::Init(images[ImageType::DOORS]);

	//Creación objetos
	FrameDoor** frameDoors = new FrameDoor * [NUM_VISIBLE_DOORS];
	Door** doors = new Door * [NUM_VISIBLE_DOORS];
	float nextClosingDoorSeconds[NUM_VISIBLE_DOORS];	//Creación del array de instantes de tiempo en los que se cierran las puertas
	float nextOpeningDoorSeconds[NUM_VISIBLE_DOORS];	//Creación del array de instantes de tiempo en los que se abren las puertas

	for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
	{
		frameDoors[i] = new FrameDoor(GAME_X_OFFSET + i * FRAME_DOOR_WIDTH, FRAME_DOOR_OFFSETY);
		doors[i] = new Door(2 * GAME_X_OFFSET + i * FRAME_DOOR_WIDTH, DOOR_OFFSET_Y + FRAME_DOOR_OFFSETY);
		nextClosingDoorSeconds[i] = 0.0f;
		nextOpeningDoorSeconds[i] = 0.0f;
	}

	//Cliente
	//Inicialización
	Client::Init(images[ImageType::CLIENT]);

	//Creación objetos
	//Client * client = new Client(32 + GAME_X_OFFSET, 24 + FRAME_DOOR_OFFSETY, door);

	//Dollar
	//Inicialización
	Dollar::Init(images[ImageType::DOLLARS]);

	//Creación objetos
	Dollar** dollars = new Dollar * [NUM_DOORS];

	for (int i = 0; i < NUM_DOORS; i++)
		dollars[i] = new Dollar(i*DOLLAR_WIDTH + GAME_X_OFFSET, 0);


	//Configuración de la escena

	//Inicializar UI
	for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
		dollars[i]->SetVisible(true);

	//Puerta seleccionada (mas a la izquierda)
	int doorIndex = 0;
	
	float gameTimer = 0.0f;

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

	//Contención inicial. No empieza el bucle principal hasta que se limpien todos los buffers
	while (RendererThread::GetPendingFrames() > 0)
		;

	//Variables bucle
	int tick = 0;
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

		if (Input::GetUserInput().Key_O)
		{
			if (doorIndex == 0)
				doorIndex = NUM_DOORS - 1;
			else
				doorIndex -= 1;

			dollars[doorIndex]->SetVisible(true);
			dollars[(doorIndex + 3) % NUM_DOORS]->SetVisible(false);
		}
		if (Input::GetUserInput().Key_P)
		{
			dollars[doorIndex]->SetVisible(false);
			doorIndex = (doorIndex + 1) % NUM_DOORS;
			dollars[(doorIndex + 2) % NUM_DOORS]->SetVisible(true);
		}

		// update game logic as lag permits
		while (lag >= timestep) 
		{
			gameTimer += deltaTime;

			for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
			{
				//Se abre una puerta
				if (doors[i]->IsClosed())
				{
					if(nextOpeningDoorSeconds[i] == 0.0f)
						nextOpeningDoorSeconds[i] = gameTimer + RandomFloat(MIN_SECONDS_OPENING_DOOR, MAX_SECONDS_OPENING_DOOR);

					if (gameTimer > nextOpeningDoorSeconds[i]) {
						doors[i]->SetClosed(false);
						nextClosingDoorSeconds[i] = 0.0f;
					}
				}

				//Se comprueba si la puerta esta abierta
				if (doors[i]->IsOpened()) 
				{
					if (nextClosingDoorSeconds[i] == 0.0f)
						nextClosingDoorSeconds[i] = gameTimer + DOOR_OPENED_TIME;

					if (gameTimer > nextClosingDoorSeconds[i])
					{
						doors[i]->SetClosed(true);
						dollars[(doorIndex + i) % NUM_DOORS]->WinMoney();
						nextOpeningDoorSeconds[i] = 0.0f;
					}
				}

			}
			//Paso de simulación
			//Puerta
			for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
			{
				frameDoors[i]->Update(tick, gameTimer);
				doors[i]->Update(tick, gameTimer);
			}
			//Dolares
			for (int i = 0; i < NUM_DOORS; i++)
				dollars[i]->Update(tick, gameTimer);

			//waves->Update();

			lag -= timestep;
		}

		//Contención. Se para la hebra si el render va muy retrasado
		while (RendererThread::GetPendingFrames() >= NUM_BUFFERS)
			;

		//Doors
		for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
		{
			frameDoors[i]->Render();
			doors[i]->Render();
		}

		//Dollars
		for (int i = 0; i < NUM_DOORS; i++)
			dollars[i]->Render();


		//rendererThread->EnqueueCommand(clearCommand); // En esta práctica no se hace clear para mantener la coherencia de frames

		//Comunicación logica con render: se le envia a la hebra de render el comando con las nuevas condiciones de la logica.
		//rendererThread->EnqueueCommand(waves->GetRenderCommand());

		RendererThread::EnqueueCommand(presentCommand);

		//Delay
		tick++;
	}

	RendererThread::Stop();

	//Dollars
	for (int i = 0; i < NUM_DOORS; ++i)
		delete[] dollars[i];

	delete[] dollars;
	dollars = nullptr;
	Dollar::Release();

	//Cliente
	Client::Release();

	//Puertas
	for (int i = 0; i < NUM_VISIBLE_DOORS; ++i) {
		delete[] doors[i];
		delete[] frameDoors[i];
	}

	//Puerta
	delete[] doors;
	doors = nullptr;
	Door::Release();

	//FrameDoor
	delete[] frameDoors;
	frameDoors = nullptr;
	FrameDoor::Release();

	//Liberar recursos
	for (int i = 0; i < ImageType::SIZE; i++)
	{
		delete images[i];
		images[i] = nullptr;
	}

	Input::Release();
	Renderer::Release();
	Platform::Release();

	return 0;
}
