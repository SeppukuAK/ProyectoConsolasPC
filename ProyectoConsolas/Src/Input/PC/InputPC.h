#pragma once
#include <SDL.h> //Utiliza la librería SDL
#include "../UserInput.h"
#include "../../Utilities/InputObserver.h"
#include <iostream>
#include <queue>

/*
	Clase con métodos estáticos.
*/
class InputPC
{
public:
	class Observer : public InputObserver
	{
	public:
		virtual bool HandleEvent(SDL_Event e) override;
	};

private:
	static SDL_GameController* gameController;	//Manejador del GameController
	static Observer observer;
	static std::queue<SDL_Event> eventQueue;
	static UserInput userInput; //Informacion del "boton" pulsado

public:

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
		GESTIONA LOS EVENTOS
	*/
	static void Tick();

	static void AddEvent(SDL_Event e)
	{
		eventQueue.push(e);
	}

	/*
		Devuelve la informacion de una entrada de input
	*/
	static UserInput GetUserInput() { return userInput; };

};


