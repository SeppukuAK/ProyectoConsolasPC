#pragma once
#include <queue>
#include "InputObserver.h"
#include "../UserInput.h"

//Declaración adelantada para evitar inclusión adicional
struct _SDL_GameController;
typedef struct _SDL_GameController SDL_GameController;
union SDL_Event;

/*
	Clase con métodos estáticos.
*/
class InputPC
{
public:
	/*
		Implementa la función que ejecutará cuando reciba un evento.
		Detecta el tipo de input y si le interesa, lo añade a InputPC
	*/
	class Observer : public InputObserver
	{
	public:
		virtual bool HandleEvent(SDL_Event e) override;
	};

private:
	static const int JOYSTICK_MAX_VALUE;
	static const int JOYSTICK_DEAD_ZONE;
	static const int TRIGGER_MAX_VALUE;

	static SDL_GameController* gameController;	//Manejador del GameController
	static Observer observer;					//Observa los eventos de Input de Platform. TODO: Puntero?????????
	static std::queue<SDL_Event> eventQueue;
	static UserInput userInput; //Informacion del "boton" pulsado

public:

	/*
		Inicializa el input.
		Se registra a platform para escuchar el input
	*/
	static void Init();

	/*
		Destruye el input
	*/
	static void Release();

	/*
		Procesa todos los eventos de la cola y los guarda en "userInput"
	*/
	static void Tick();

	/*
		Devuelve la informacion de una entrada de input
	*/
	inline static UserInput GetUserInput() { return userInput; };

	/*
		Añade un evento a la cola de input para que se procese
	*/
	static void AddEvent(SDL_Event e);

};


