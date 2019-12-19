#pragma once

class Image;
class FrameDoor;
class Door;
class Dollar;
class DeathBackground;
class Bang;

class WestBank
{
private:
	/*
		Imágenes disponibles en la escena
	*/
	enum ImageType { DOOR_FRAME, DOORS, CLIENT, THIEF, DOLLARS, BANG, SIZE };

	/*
		Posibles estados de la escena
	*/
	enum GameState { GAMEPLAY, SCROLL, DEATH };

	//Game
	static const float MIN_SECONDS_OPENING_DOOR;
	static const float MAX_SECONDS_OPENING_DOOR;
	static const float DOOR_OPENED_TIME;	//Incluye el tiempo abriendose
	static const float BANG_TIME;	

	static const int NUM_DOORS;
	static const int NUM_VISIBLE_DOORS;

	static const int GAME_X_OFFSET;

	//Door
	static const int DOOR_OFFSET_Y;

	//Carga de recursos. Asumen una resolución de juego de 640 x 360
	static Image** images;

	//Puertas
	static FrameDoor** frameDoors;			//Pared con puerta. 3 De ellas consecutivas crean la escena principal de juego
	static Door** doors;
	static float nextClosingDoorSeconds;	//Instante de tiempo en los que se cierra las puerta
	static float nextOpeningDoorSeconds;	//Instante de tiempo en los que se abre las puerta
	static int doorChosenIndex;				//Puerta elegida para abrirse
	static int oldDoorChosenIndex;			//Anterior Puerta elegida para abrirse
	static bool allDoorsClosed;

	//UI
	static Dollar** dollars;
	static DeathBackground * deathBackground;
	static Bang* bang;

	//Atributos escena
	static GameState gameState;			//Estado actual de la escena
	static int doorIndex;				//Puerta seleccionada (mas a la izquierda)

public:
	/*
		Inicializa los recursos del juego y la escena
	*/
	static void Init();

	/*
		Libera los recuros del juego y memoria de objetos creados
	*/
	static void Release();

	static void Input();
	static void Update(float time, float tick);
	static void Render();

private:

	/*
		Carga las imágenes que utiliza el juego
	*/
	static void LoadResources();

	/*
		Inicializa atributos y objetos del juego
	*/
	static void InitScene();

	/*
		Devuelve un float entre el rango pasado
	*/
	static float RandomFloat(float a, float b);
};

