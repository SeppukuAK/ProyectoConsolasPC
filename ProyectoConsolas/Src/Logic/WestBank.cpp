#include "WestBank.h"
#include <stdlib.h>		/* srand, rand */
#include "../Renderer/Image.h"
#include "../Platform/Platform.h"
#include "../Input/Input.h"
#include "FrameDoor.h"
#include "Door.h"
#include "Client.h"
#include "Dollar.h"
#include "DeathBackground.h"
#include "Bang.h"

#include <iostream>
using namespace std;

//Game
const float WestBank::MIN_SECONDS_OPENING_DOOR = 0.5f;
const float WestBank::MAX_SECONDS_OPENING_DOOR = 3.0f;
const float WestBank::DOOR_OPENED_TIME = 2.0f;	//Incluye el tiempo abriendose
const float WestBank::BANG_TIME = 2.0f;

const int WestBank::NUM_DOORS = 9;
const int WestBank::NUM_VISIBLE_DOORS = 3;

const int WestBank::GAME_X_OFFSET = 32;

//Door
const int WestBank::DOOR_OFFSET_Y = 24;

//Inicialización atributos estáticos
int WestBank::posX = 0;
Image** WestBank::images = nullptr;
FrameDoor** WestBank::frameDoors = nullptr;
Door** WestBank::doors = nullptr;
float WestBank::nextClosingDoorSeconds = 0.0f;
float WestBank::nextOpeningDoorSeconds = 0.0f;
int WestBank::doorChosenIndex = 0;
int WestBank::oldDoorChosenIndex = -1;
bool WestBank::allDoorsClosed = true;
Dollar** WestBank::dollars = nullptr;
Bang* WestBank::bang = nullptr;
DeathBackground* WestBank::deathBackground = nullptr;
WestBank::GameState WestBank::gameState = WestBank::GameState::GAMEPLAY;
int WestBank::doorIndex = 0;

void WestBank::Init()
{
	LoadResources();
	InitScene();
}

void WestBank::Release()
{
	//Bang
	delete bang;
	bang = nullptr;
	Bang::Release();

	delete deathBackground;
	deathBackground = nullptr;

	//Dollars
	for (int i = 0; i < NUM_DOORS; ++i)
		delete dollars[i];

	delete[] dollars;
	dollars = nullptr;
	Dollar::Release();

	//Cliente
	Client::Release();

	//Puertas
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
	images = new Image *[ImageType::SIZE];

	images[ImageType::DOOR_FRAME] = Platform::LoadImage("SpritesSheetsWestBank/marcoPuerta.rgba");
	images[ImageType::DOORS] = Platform::LoadImage("SpritesSheetsWestBank/puertas.rgba");
	images[ImageType::CLIENT] = Platform::LoadImage("SpritesSheetsWestBank/client.rgba");
	images[ImageType::THIEF] = Platform::LoadImage("SpritesSheetsWestBank/ladron.rgba");
	images[ImageType::DOLLARS] = Platform::LoadImage("SpritesSheetsWestBank/dolares.rgba");
	images[ImageType::BANG] = Platform::LoadImage("SpritesSheetsWestBank/bang.rgba");
}

void WestBank::InitScene()
{
	//Puertas
	//Inicialización
	FrameDoor::Init(images[ImageType::DOOR_FRAME]);
	Door::Init(images[ImageType::DOORS]);

	//Creación objetos
	frameDoors = new FrameDoor *[NUM_VISIBLE_DOORS];
	doors = new Door *[NUM_VISIBLE_DOORS];

	float frameDoorWidth = images[ImageType::DOOR_FRAME]->GetWidth();
	float frameDoorOffsetY = images[ImageType::DOLLARS]->GetHeight();

	for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
	{
		frameDoors[i] = new FrameDoor(GAME_X_OFFSET + i * frameDoorWidth, frameDoorOffsetY);
		doors[i] = new Door(2 * GAME_X_OFFSET + i * frameDoorWidth, DOOR_OFFSET_Y + frameDoorOffsetY);
	}

	//Cliente
	//Inicialización
	Client::Init(images[ImageType::CLIENT]);

	//Creación objetos
	//Client * client = new Client(32 + GAME_X_OFFSET, 24 + frameDoorOffsetY, door);

	//Dollar
	//Inicialización
	Dollar::Init(images[ImageType::DOLLARS]);

	//Creación objetos
	dollars = new Dollar *[NUM_DOORS];

	float dollarWidth = images[ImageType::DOLLARS]->GetWidth() / Dollar::NUM_SPRITES;

	for (int i = 0; i < NUM_DOORS; i++)
		dollars[i] = new Dollar(i * dollarWidth + GAME_X_OFFSET, 0);

	deathBackground = new DeathBackground();

	Bang::Init(images[ImageType::BANG]);
	bang = new Bang(200, 200, BANG_TIME);

	//Configuración de la escena
	doorChosenIndex = rand() % NUM_VISIBLE_DOORS;

	//Inicializar UI
	for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
		dollars[i]->SetVisible(true);
}

void WestBank::Input()
{
	if (gameState == GameState::GAMEPLAY)
	{
		//Scroll
		if (allDoorsClosed)
		{
			if (Input::GetUserInput().Key_O)
			{
				if (doorIndex == 0)
					doorIndex = NUM_DOORS - 1;
				else
					doorIndex -= 1;

				dollars[doorIndex]->SetVisible(true);
				dollars[(doorIndex + 3) % NUM_DOORS]->SetVisible(false);
				gameState = GameState::SCROLL_LEFT;
				posX = 0;
			}
			if (Input::GetUserInput().Key_P)
			{
				dollars[doorIndex]->SetVisible(false);
				doorIndex = (doorIndex + 1) % NUM_DOORS;
				dollars[(doorIndex + 2) % NUM_DOORS]->SetVisible(true);
				gameState = GameState::SCROLL_RIGHT;
				posX = FrameDoor::GetFrameDoorWidth();

			}
		}
		if (Input::GetUserInput().Key_1)
		{
			gameState = WestBank::GameState::DEATH;
		}

	//	cout << Input::GetUserInput().HorizontalAxis << endl;

	}
}

void WestBank::Update(float time, float tick)
{
	switch (gameState)
	{
	case GameState::GAMEPLAY:
	{

		if (doors[doorChosenIndex]->IsClosed())
		{
			//Si no se ha planificado cuando se tiene que abrir una puerta, se planifica
			if (nextOpeningDoorSeconds == 0.0f)
				nextOpeningDoorSeconds = time + RandomFloat(MIN_SECONDS_OPENING_DOOR, MAX_SECONDS_OPENING_DOOR);

			//A la puerta le toca abrirse
			else if (time > nextOpeningDoorSeconds) {
				doors[doorChosenIndex]->SetClosed(false);
				nextOpeningDoorSeconds = 0.0f;
				nextClosingDoorSeconds = 0.0f;
			}
			//Si a la puerta actual no le toca abrirse y la anterior está cerrada, están todas cerradas
			else if(oldDoorChosenIndex >= 0 && doors[oldDoorChosenIndex]->IsClosed())
			{
				allDoorsClosed = true;
				oldDoorChosenIndex = -1;
			}
		}

		//Se comprueba si la puerta esta abierta
		else if (doors[doorChosenIndex]->IsOpened())
		{
			//Si no se ha planificado el cierre, se planifica
			if (nextClosingDoorSeconds == 0.0f)
				nextClosingDoorSeconds = time + DOOR_OPENED_TIME;

			//A la puerta le toca cerrarse(está abierta)
			else if (time > nextClosingDoorSeconds)
			{
				doors[doorChosenIndex]->SetClosed(true);
				dollars[(doorIndex + doorChosenIndex) % NUM_DOORS]->WinMoney();
				nextOpeningDoorSeconds = 0.0f;
				nextClosingDoorSeconds = 0.0f;
				allDoorsClosed = true;

				//Se elige la nueva puerta
				oldDoorChosenIndex = doorChosenIndex;
				doorChosenIndex = rand() % NUM_VISIBLE_DOORS;
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

		break;
	}
	case GameState::SCROLL_LEFT:
	{
		//Dolares
		for (int i = 0; i < NUM_DOORS; i++)
			dollars[i]->Update(tick, time);

		posX++;//Se aumenta la posicion del frameDoor
		if (posX == FrameDoor::GetFrameDoorWidth())
			gameState = GameState::GAMEPLAY;

		break;
	}
	case GameState::SCROLL_RIGHT:
	{
		//Dolares
		for (int i = 0; i < NUM_DOORS; i++)
			dollars[i]->Update(tick, time);
		posX--;
		if (posX == 0)
			gameState = GameState::GAMEPLAY;

		break;
	}
	case GameState::DEATH:
	{
		deathBackground->Update(tick, time);
		bang->Update(tick, time);
		break;
	}
	}
}

void WestBank::Render()
{
	switch (gameState)
	{
	case GameState::GAMEPLAY:
	{
		//Doors y FrameDoors
		for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
		{
			frameDoors[i]->Render();
			doors[i]->Render();		//TODO: La zona abierta no debería pintarse, sino sustituirse por el personaje que abre la puerta
		}

		//Dollars
		for (int i = 0; i < NUM_DOORS; i++)
			dollars[i]->Render();

		break;
	}
	case GameState::SCROLL_LEFT:
	case GameState::SCROLL_RIGHT:
	{
		//Dollars
		for (int i = 0; i < NUM_DOORS; i++)
			dollars[i]->Render();

		for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
		{
			frameDoors[i]->RenderWithDelta(posX);

		}
		break;
	}
	case GameState::DEATH:
	{
		deathBackground->Render();
		bang->Render();
		break;
	}
	}
}

float WestBank::RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
