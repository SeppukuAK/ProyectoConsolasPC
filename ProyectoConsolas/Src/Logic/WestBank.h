#pragma once
#include <vector>

class Image;
class FrameDoor;
class Door;
class Dollar;
class DeathBackground;
class Bang;
class Client;
class Thief;

/*

*/
class WestBank
{
public:
	static const float BANG_TIME;

private:
	/*
		Im�genes disponibles en la escena
	*/
	enum ImageType { DOOR_FRAME, DOORS, CLIENT, THIEF, DOLLARS, BANG, SIZE };

	/*
		Posibles estados de la escena
	*/
	enum GameState { GAMEPLAY, SCROLL_LEFT, SCROLL_RIGHT, INITIALIZE, DEATH };

	/*
	Tipos de personas
	*/
	enum PersonType { TYPE_CLIENT, TYPE_THIEF, TYPE_SIZE };

	//Game
	static const int GAME_WIDTH;
	static const int GAME_HEIGHT;
	static const int NUM_DOORS;
	static const int NUM_VISIBLE_DOORS;

	//Attributes
	static const float MIN_SECONDS_OPENING_DOOR;
	static const float MAX_SECONDS_OPENING_DOOR;
	static const float DOOR_OPENED_TIME;	//Incluye el tiempo abriendose
	static const float FIRE_RATE;
	static const float INITIALIZE_RATE;//Tiempo de espera antes de empezar el juego

	//Input

	//Hay una zona del joystick en la que el disparo no es valido
	static const float DEADZONE_AXIS_CENTER;		//Reposo
	static const float DEADZONE_AXIS_DIRECTION;		//Izquierda o derecha
	static const float DEADZONE_TRIGGER_ACTIVE;
	static const float DEADZONE_TRIGGER_REPOSE;


	static const int GAME_X_OFFSET;

	//Door
	static const int DOOR_OFFSET_Y;

	//FrameDoor
	static int posX;

	//Carga de recursos. Asumen una resoluci�n de juego de 640 x 360
	static Image** images;

	static FrameDoor** frameDoors;			//Pared con puerta. 3 De ellas consecutivas crean la escena principal de juego
	static Door** doors;
	static Dollar** dollars;
	static DeathBackground* deathBackground;
	static Bang* bang;

	//Client
	static std::vector <Client*> clients; //Vector de clientes(puede haber m�s de uno a la vez)

	//Thief
	static std::vector<Thief*> thieves; //Vector de clientes(puede haber m�s de uno a la vez)

	static int personChosen;		//Persona elegida (Cliente o Bandido)
	static int oldPersonChosen;		//Anterior persona elegida(Cliente o Bandido)
	//Puertas
	static float nextClosingDoorSeconds;	//Instante de tiempo en los que se cierra las puerta
	static float nextOpeningDoorSeconds;	//Instante de tiempo en los que se abre las puerta
	static int doorChosenIndex;				//Puerta elegida para abrirse
	static int oldDoorChosenIndex;			//Anterior Puerta elegida para abrirse
	static bool allDoorsClosed;

	//Dolares
	static int dollarsWon;

	//UI


	//Atributos escena
	static GameState gameState;			//Estado actual de la escena
	static int doorIndex;				//Puerta seleccionada (mas a la izquierda)
	static float nextResetGameSeconds;	//Instante de tiempo en que se resetear� el juego
	static float nextInitializeSeconds; //Instante de tiempo en que se inicia el juego
	//Input
	static float nextFire;				//Instante de tiempo en el que se puede disparar
	static bool canShoot;

public:
	/*
		Inicializa los recursos del juego y la escena
	*/
	static void Init();

	/*
		Libera los recuros del juego y memoria de objetos creados
	*/
	static void Release();

	/*
		Detección disparo y cambio de puertas en estado de juego
	*/
	static void Input();
	static void Update(float tick);
	static void Render();

private:

	/*
		Carga las im�genes que utiliza el juego
	*/
	static void LoadResources();

	/*
		Inicializa atributos y objetos del juego
	*/
	static void InitScene();

	static void ResetScene();

	//Input
	static void CheckMovement();

	/*
		Detección de disparo y recarga
	*/
	static void CheckShoot();

	/*
		Devuelve un float entre el rango pasado
	*/
	static float RandomFloat(float a, float b);
};

