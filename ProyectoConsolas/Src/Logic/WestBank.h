#pragma once

class Image;
class FrameDoor;
class Door;
class Dollar;

class WestBank
{
private:
	enum ImageType { DOOR_FRAME, DOORS, CLIENT, THIEF, DOLLARS, BANG, SIZE };

	//Game
	static const float MIN_SECONDS_OPENING_DOOR;
	static const float MAX_SECONDS_OPENING_DOOR;
	static const float DOOR_OPENED_TIME;	//Incluye el tiempo abriendose

	static const int NUM_DOORS;
	static const int NUM_VISIBLE_DOORS;

	static const int GAME_X_OFFSET;

	//FrameDoor
	static const int FRAME_DOOR_WIDTH;
	static const int FRAME_DOOR_OFFSETY;

	//Door
	static const int DOOR_ANIM_FRAMES;
	static const int DOOR_OFFSET_Y;

	//Dollar
	static const int DOLLAR_WIDTH;

	//Carga de recursos. Asumen una resolución de juego de 640 x 360
	static Image** images;

	//Puertas
	static FrameDoor** frameDoors;
	static Door** doors;
	static float* nextClosingDoorSeconds;	//Creación del array de instantes de tiempo en los que se cierran las puertas
	static float* nextOpeningDoorSeconds;	//Creación del array de instantes de tiempo en los que se abren las puertas

	//UI
	static Dollar** dollars;

	//Atributos escena
	static int doorIndex;		//Puerta seleccionada (mas a la izquierda)

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

