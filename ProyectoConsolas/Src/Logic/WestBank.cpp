#include "WestBank.h"
#include <stdlib.h>		/* srand, rand */
#include "../Renderer/Image.h"
#include "../Platform/Platform.h"
#include "../Input/Input.h"
#include "FrameDoor.h"
#include "Door.h"
#include "Client.h"
#include "Thief.h"
#include "Dollar.h"
#include "DeathBackground.h"
#include "Bang.h"
#include "../Utilities/Time.h"
#include "../Renderer/Renderer.h"

#include <iostream>
using namespace std;

//Game
const float WestBank::MIN_SECONDS_OPENING_DOOR = 0.5f;
const float WestBank::MAX_SECONDS_OPENING_DOOR = 3.0f;
const float WestBank::DOOR_OPENED_TIME = 2.0f;	//Incluye el tiempo abriendose
const float WestBank::BANG_TIME = 2.0f;
const float WestBank::FIRE_RATE = 1.0f;
const float WestBank::DEADZONE_TRIGGER_ACTIVE = 0.5f;
const float WestBank::DEADZONE_TRIGGER_REPOSE = -0.5f;

const float WestBank::DEADZONE_AXIS_CENTER = 0.2f;
const float WestBank::DEADZONE_AXIS_DIRECTION = 0.8f;


const int WestBank::NUM_DOORS = 9;
const int WestBank::NUM_VISIBLE_DOORS = 3;

const int WestBank::GAME_X_OFFSET = 32;

//Door
const int WestBank::DOOR_OFFSET_Y = 24;

//Dollar
int WestBank::dollarsWon = 0;

//Inicializaci�n atributos est�ticos
int WestBank::posX = 0;
Image** WestBank::images = nullptr;

FrameDoor** WestBank::frameDoors = nullptr;
Door** WestBank::doors = nullptr;
Dollar** WestBank::dollars = nullptr;
DeathBackground* WestBank::deathBackground = nullptr;
Bang* WestBank::bang = nullptr;

//Client
vector<Client*> WestBank::clients(NUM_VISIBLE_DOORS);
vector<Thief*> WestBank::thieves(NUM_VISIBLE_DOORS);
int WestBank::personChosen = -1;

float WestBank::nextClosingDoorSeconds = 0.0f;
float WestBank::nextOpeningDoorSeconds = 0.0f;
int WestBank::doorChosenIndex = 0;
int WestBank::oldDoorChosenIndex = -1;
bool WestBank::allDoorsClosed = true;

int WestBank::doorIndex = 0;

WestBank::GameState WestBank::gameState = GameState::GAMEPLAY;
float WestBank::nextResetGameSeconds = 0.0f;
float WestBank::nextFire = 0.0f;
bool WestBank::canShoot = true;

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

	//Cliente y Thief
	Client::Release();
	Thief::Release();

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

	for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
	{
		if (clients[i] != nullptr)
			 clients[i] =nullptr;
		if (thieves[i] != nullptr)
			 thieves[i] = nullptr;
	}
	//delete[]clients;
	//delete[]thieves;

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
	//Inicializaci�n
	FrameDoor::Init(images[ImageType::DOOR_FRAME]);
	Door::Init(images[ImageType::DOORS]);

	//Creaci�n objetos
	frameDoors = new FrameDoor * [NUM_VISIBLE_DOORS];
	doors = new Door * [NUM_VISIBLE_DOORS];

	float frameDoorWidth = images[ImageType::DOOR_FRAME]->GetWidth();
	float frameDoorOffsetY = images[ImageType::DOLLARS]->GetHeight();

	for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
	{
		frameDoors[i] = new FrameDoor(GAME_X_OFFSET + i * frameDoorWidth, frameDoorOffsetY);
		doors[i] = new Door(2 * GAME_X_OFFSET + i * frameDoorWidth, DOOR_OFFSET_Y + frameDoorOffsetY);
	}

	//Cliente
	//Inicializaci�n
	Client::Init(images[ImageType::CLIENT]);

	Thief::Init(images[ImageType::THIEF]);

	//Creaci�n objetos

	//Dollar
	//Inicializaci�n
	Dollar::Init(images[ImageType::DOLLARS]);

	//Creaci�n objetos
	dollars = new Dollar * [NUM_DOORS];

	float dollarWidth = images[ImageType::DOLLARS]->GetWidth() / Dollar::NUM_SPRITES;

	for (int i = 0; i < NUM_DOORS; i++)
		dollars[i] = new Dollar(i * dollarWidth + GAME_X_OFFSET, 0);

	deathBackground = new DeathBackground();

	Bang::Init(images[ImageType::BANG]);
	bang = new Bang(200, 200);

	Renderer::SetScale(Renderer::GetWidth() / (NUM_VISIBLE_DOORS * frameDoorWidth));

	ResetScene();
}

void WestBank::ResetScene()
{
	gameState = GameState::GAMEPLAY;
	deathBackground->Reset();
	bang->Reset();

	for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
	{
		frameDoors[i]->Reset();
		doors[i]->Reset();

		//Clientes y ladrones
		if (clients[i] != nullptr) {
			cout << "cliente borrado" << endl;
			clients[i]->Reset();//TODO: Supongo?
		}

		if (thieves[i] != nullptr) {
			cout << "ladron borrado" << endl;

			thieves[i]->Reset();
		}
	}
	for (int i = 0; i < NUM_DOORS; i++)
	{
		dollars[i]->Reset();
	}

	//Inicializar UI
	for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
		dollars[i + doorIndex]->SetVisible(true);

	nextResetGameSeconds = 0.0f;

	//Configuraci�n de la escena
	doorChosenIndex = rand() % NUM_VISIBLE_DOORS;
	personChosen = 0;//2 = typeOfPerson ( 0 = Cliente / 1 = Bandido )

	nextClosingDoorSeconds = 0.0f;
	nextOpeningDoorSeconds = 0.0f;
	dollarsWon = 0;
	oldDoorChosenIndex = -1;
	allDoorsClosed = true;
	nextFire = 0.0f;
	canShoot = true;
}

void WestBank::Input()
{
	if (gameState == GameState::GAMEPLAY)
	{
		//Scroll
		if (allDoorsClosed)
		{
			if (Input::GetUserInput().L1 || Input::GetUserInput().Key_O)
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

			else if (Input::GetUserInput().R1 || Input::GetUserInput().Key_P)
			{
				dollars[doorIndex]->SetVisible(false);
				doorIndex = (doorIndex + 1) % NUM_DOORS;
				dollars[(doorIndex + 2) % NUM_DOORS]->SetVisible(true);
				gameState = GameState::SCROLL_RIGHT;
				posX = FrameDoor::GetFrameDoorWidth();

			}
		}

		//Recarga. Si se han soltado todas las teclas de disparo y los triggers de disparo y ha pasado el tiempo necesario, puedo disparar
		if (Input::GetUserInput().L2 < DEADZONE_TRIGGER_REPOSE && Input::GetUserInput().R2 < DEADZONE_TRIGGER_REPOSE
			&& !Input::GetUserInput().Key_1 && !Input::GetUserInput().Key_2 && !Input::GetUserInput().Key_3
			&& Time::time > nextFire)
		{
			canShoot = true;
		}

		if (canShoot)
		{
			int dir = -1;	//Disparo no valido

			//Input de mando
			if (Input::GetUserInput().L2 > DEADZONE_TRIGGER_ACTIVE || Input::GetUserInput().R2 > DEADZONE_TRIGGER_ACTIVE)
			{
				float horizontalAxis = Input::GetUserInput().HorizontalAxis;

				//Centro --> [-DEADZONE_AXIS_CENTER,DEADZONE_AXIS_CENTER]
				if (horizontalAxis > -DEADZONE_AXIS_CENTER && horizontalAxis < DEADZONE_AXIS_CENTER)
					dir = 1;

				//Izquierda --> [-1,-DEADZONE_AXIS_DIRECTION]
				else if (horizontalAxis < -DEADZONE_AXIS_DIRECTION)
					dir = 0;

				//Derecha --> [DEADZONE_AXIS_DIRECTION,1
				else if (horizontalAxis > DEADZONE_AXIS_DIRECTION)
					dir = 2;

			}

			//Centro 
			else if (Input::GetUserInput().Key_2)
				dir = 1;

			//Izquierda
			else if (Input::GetUserInput().Key_1)
				dir = 0;

			//Derecha 
			else if (Input::GetUserInput().Key_3)
				dir = 2;

			//Direccion valida
			if (dir != -1)
			{
				canShoot = false;
				nextFire = Time::time + FIRE_RATE;

				cout << "FIRE: " << dir << endl;
			}
		}
	}
}

void WestBank::Update(float tick)
{
	switch (gameState)
	{
	case GameState::GAMEPLAY:
	{
		if (oldDoorChosenIndex >= 0 && doors[oldDoorChosenIndex]->IsClosed()) {

			//TODO: cutre pero no se como hacerlo
			if (personChosen == 0) {
				cout << "Se elimina al cliente de la puerta: " << oldDoorChosenIndex << endl;

				clients[oldDoorChosenIndex] = nullptr;
			}
			else {
				cout << "Se elimina al ladron de la puerta: " << oldDoorChosenIndex << endl;

				thieves[oldDoorChosenIndex] = nullptr;
			}

			oldDoorChosenIndex = -1;

		}
		if (doors[doorChosenIndex]->IsClosed())
		{
			//Si no se ha planificado cuando se tiene que abrir una puerta, se planifica
			if (nextOpeningDoorSeconds == 0.0f) {
				nextOpeningDoorSeconds = Time::time + RandomFloat(MIN_SECONDS_OPENING_DOOR, MAX_SECONDS_OPENING_DOOR);

			}

			//A la puerta le toca abrirse
			//Se decide si toca cliente o bandido
			else if (Time::time > nextOpeningDoorSeconds) {
				allDoorsClosed = false;
				doors[doorChosenIndex]->SetClosed(false);
				nextOpeningDoorSeconds = 0.0f;
				nextClosingDoorSeconds = 0.0f;

				if (personChosen == 0) {
					Client* client = new Client(doors[doorChosenIndex]->GetPosX(), doors[doorChosenIndex]->GetPosY(), doors[doorChosenIndex]);
					clients[doorChosenIndex] = client;//Se añade el nuevo cliente seleccionado
					cout << "Se crea cliente en la puerta: " << doorChosenIndex << endl;

				}
				else {
					Thief* thief = new Thief(doors[doorChosenIndex]->GetPosX(), doors[doorChosenIndex]->GetPosY(), doors[doorChosenIndex]);
					thieves[doorChosenIndex] = thief;//Se añade el nuevo cliente seleccionado
					cout << "Se crea ladron en la puerta: " << doorChosenIndex << endl;


				}
			}

			//Si a la puerta actual no le toca abrirse y la anterior est� cerrada, est�n todas cerradas
			else if (oldDoorChosenIndex == -1)
			{
				allDoorsClosed = true;
			}
		}


		//Se comprueba si la puerta esta abierta
		else if (doors[doorChosenIndex]->IsOpened())
		{
			//Si no se ha planificado el cierre, se planifica
			if (nextClosingDoorSeconds == 0.0f)
				nextClosingDoorSeconds = Time::time + DOOR_OPENED_TIME;

			//A la puerta le toca cerrarse(est� abierta)
			else if (Time::time > nextClosingDoorSeconds)
			{
				doors[doorChosenIndex]->SetClosed(true);

				//Se gana un dolar
				if (personChosen == 0) {

					//Si no se ha ganado este dolar ya, se aumenta el contador
					if (!dollars[(doorIndex + doorChosenIndex) % NUM_DOORS]->HasAlreadyWonDollar()) {
						dollarsWon++;
						if (dollarsWon == NUM_DOORS) {
							ResetScene();
						}
						cout << dollarsWon << endl;
					}
					dollars[(doorIndex + doorChosenIndex) % NUM_DOORS]->WinMoney();

					
				}

				//Te mueres porque te dispara el ladron
				else
					gameState = WestBank::GameState::DEATH;

				nextOpeningDoorSeconds = 0.0f;
				nextClosingDoorSeconds = 0.0f;

				//Se elige la nueva puerta
				oldDoorChosenIndex = doorChosenIndex;
				doorChosenIndex = rand() % NUM_VISIBLE_DOORS;
				personChosen = 0;//2 = typeOfPerson ( 0 = Cliente / 1 = Bandido )
			}
		}

		//Paso de simulaci�n
		//Puerta
		for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
		{
			frameDoors[i]->Update(tick);
			doors[i]->Update(tick);

			//Compruebo si existe el cliente
			if (clients[i] != nullptr)
				clients[i]->Update(tick);

			//Compruebo si existe el cliente
			if (thieves[i] != nullptr)
				thieves[i]->Update(tick);

		}

		//Dolares
		for (int i = 0; i < NUM_DOORS; i++)
			dollars[i]->Update(tick);

		break;
	}
	case GameState::SCROLL_LEFT:
	{
		//Dolares
		for (int i = 0; i < NUM_DOORS; i++)
			dollars[i]->Update(tick);

		posX += 2;//Se aumenta la posicion del frameDoor
		if (posX > FrameDoor::GetFrameDoorWidth() - 1)
		{
			for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
			{
				frameDoors[i]->Reset();

			}
			gameState = GameState::GAMEPLAY;
		}

		break;
	}
	case GameState::SCROLL_RIGHT:
	{
		//Dolares
		for (int i = 0; i < NUM_DOORS; i++)
			dollars[i]->Update(tick);

		posX -= 2;

		if (posX <= 0)
		{
			for (int i = 0; i < NUM_VISIBLE_DOORS; i++)
				frameDoors[i]->Reset();

			gameState = GameState::GAMEPLAY;
		}

		break;
	}
	case GameState::DEATH:
	{
		if (nextResetGameSeconds == 0.0f)
		{
			nextResetGameSeconds = Time::time + BANG_TIME;
		}

		else if (Time::time >= nextResetGameSeconds)
		{
			ResetScene();
		}
		else
		{
			deathBackground->Update(tick);
			bang->Update(tick);
		}

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
			doors[i]->Render();		//TODO: La zona abierta no deber�a pintarse, sino sustituirse por el personaje que abre la puerta
			//Compruebo si existe el cliente que quiero renderizar
			if (clients[i] != nullptr)
				clients[i]->Render();

			//Compruebo si existe el ladron que quiero renderizar
			if (thieves[i] != nullptr)
				thieves[i]->Render();
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
