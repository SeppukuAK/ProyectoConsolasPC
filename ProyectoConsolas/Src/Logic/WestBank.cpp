#include "WestBank.h"
#include <stdlib.h>		/* srand, rand */
#include "../Renderer/Image.h"
#include "../Platform/Platform.h"
#include "../Input/Input.h"
#include "FrameDoor.h"
#include "Door.h"
#include "Client.h"
#include "Dollar.h"


//Game
const float WestBank::MIN_SECONDS_OPENING_DOOR = 4.0f;
const float WestBank::MAX_SECONDS_OPENING_DOOR = 10.0f;
const float WestBank::DOOR_OPENED_TIME = 2.0f;	//Incluye el tiempo abriendose

const int WestBank::NUM_DOORS = 9;
const int WestBank::NUM_VISIBLE_DOORS = 3;

const int WestBank::GAME_X_OFFSET = 32;

//FrameDoor
const int WestBank::FRAME_DOOR_WIDTH = 192;
const int WestBank::FRAME_DOOR_OFFSETY = 48;

//Door
const int WestBank::DOOR_ANIM_FRAMES = 4;
const int WestBank::DOOR_OFFSET_Y = 24;

//Dollar
const int WestBank::DOLLAR_WIDTH = 64;

//Inicialización atributos estáticos
Image** WestBank::images = nullptr;
FrameDoor** WestBank::frameDoors = nullptr;
Door** WestBank::doors = nullptr;
float* WestBank::nextClosingDoorSeconds = nullptr;
float* WestBank::nextOpeningDoorSeconds = nullptr;
Dollar** WestBank::dollars = nullptr;
int WestBank::doorIndex = 0;

void WestBank::Init()
{
	LoadResources();
	InitScene();
}

void WestBank::Release()
{
	//Dollars
	for (int i = 0; i < NUM_DOORS; ++i)
		delete dollars[i];

	delete[] dollars;
	dollars = nullptr;
	Dollar::Release();

	//Cliente
	Client::Release();

	//Puertas
	delete nextClosingDoorSeconds;
	nextClosingDoorSeconds = nullptr;

	delete nextOpeningDoorSeconds;
	nextOpeningDoorSeconds = nullptr;

	for (int i = 0; i < NUM_VISIBLE_DOORS; ++i)
	{
		delete doors[i];
		delete frameDoors[i];
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
		delete images[i];

	delete[] images;
	images = nullptr;
}

void WestBank::LoadResources()
{
	images = new Image * [ImageType::SIZE];
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

void WestBank::InitScene()
{
	//Puertas
	//Inicialización
	FrameDoor::Init(images[ImageType::DOOR_FRAME]);
	Door::Init(images[ImageType::DOORS]);

	//Creación objetos
	frameDoors = new FrameDoor * [NUM_VISIBLE_DOORS];
	doors = new Door * [NUM_VISIBLE_DOORS];
	nextClosingDoorSeconds = new float[NUM_VISIBLE_DOORS];
	nextOpeningDoorSeconds = new float[NUM_VISIBLE_DOORS];

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
	dollars = new Dollar * [NUM_DOORS];

	for (int i = 0; i < NUM_DOORS; i++)
		dollars[i] = new Dollar(i * DOLLAR_WIDTH + GAME_X_OFFSET, 0);

	//Configuración de la escena

	//Inicializar UI
	for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
		dollars[i]->SetVisible(true);
}

void WestBank::Input()
{
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

}

void WestBank::Update(float time, float tick)
{
	for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
	{
		//Se abre una puerta
		if (doors[i]->IsClosed())
		{
			if (nextOpeningDoorSeconds[i] == 0.0f)
				nextOpeningDoorSeconds[i] = time + RandomFloat(MIN_SECONDS_OPENING_DOOR, MAX_SECONDS_OPENING_DOOR);

			if (time > nextOpeningDoorSeconds[i]) {
				doors[i]->SetClosed(false);
				nextClosingDoorSeconds[i] = 0.0f;
			}
		}

		//Se comprueba si la puerta esta abierta
		if (doors[i]->IsOpened())
		{
			if (nextClosingDoorSeconds[i] == 0.0f)
				nextClosingDoorSeconds[i] = time + DOOR_OPENED_TIME;

			if (time > nextClosingDoorSeconds[i])
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
		frameDoors[i]->Update(tick, time);
		doors[i]->Update(tick, time);
	}
	//Dolares
	for (int i = 0; i < NUM_DOORS; i++)
		dollars[i]->Update(tick, time);
}

void WestBank::Render()
{
	//Doors
	for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
	{
		frameDoors[i]->Render();
		doors[i]->Render();
	}

	//Dollars
	for (int i = 0; i < NUM_DOORS; i++)
		dollars[i]->Render();
}

float WestBank::RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
