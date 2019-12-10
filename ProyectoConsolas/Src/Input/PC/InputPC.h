#pragma once
#include <SDL.h> //Utiliza la librería SDL
#include "../UserInput.h"
/*
	Clase con métodos estáticos. 
*/
class InputPC
{
private:

	static SDL_GameController* gameController;
public:

	//Analog joystick dead zone
	static const int JOYSTICK_DEAD_ZONE = 8000; //TODO: ESTE ES EL DEADZONE PARA LOS STICKS, CUAL ES EL DEADZONE PARA LOS TRIGGERS?
	static UserInput userInput; //Informacion del "boton" pulsado

	/*
		Inicializa el input
	*/
	static void Init();

	/*
		Destruye el input
	*/
	static void Release();

	/*
	Devuelve un bool que indica si el usuario ha decidido terminar la aplicación.
	Devuelve true si se ha pulsado la 'X', cualquier tecla o el ratón
	*/
	static void Tick();

	/*
	Devuelve la informacion de una entrada de input
	*/
	static UserInput GetUserInput() { return userInput; };

	
};


