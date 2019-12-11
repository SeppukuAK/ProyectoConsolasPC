#pragma once
#include <SDL.h> //Utiliza la librería SDL
#include "../UserInput.h"
#include "../../Utilities/InputObserver.h"
#include "../../Platform/Platform.h"
#include <iostream>
#include <queue>

/*
	Clase con métodos estáticos.
*/
class InputPC
{
public:
	class Listener : public InputObserver
	{
	public:
		Listener(){ PlatformPC::AddObserver(this); }
		~Listener(){ PlatformPC::RemoveObserver(this); }
		virtual bool HandleEvent(SDL_Event e) override
		{
			return InputPC::AddEvent(e);
		}
	};

private:
	static SDL_GameController* gameController;	//Manejador del GameController
	static Listener listener;
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

	static bool AddEvent(SDL_Event e)
	{
		eventQueue.push(e);
		return true;
	}

	/*
		Devuelve la informacion de una entrada de input
	*/
	static UserInput GetUserInput() { return userInput; };

};


