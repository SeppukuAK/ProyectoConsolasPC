#pragma once
#include <queue>
#include "InputObserver.h"
#include "../UserInput.h"

//Declaraci�n adelantada para evitar inclusi�n adicional
struct _SDL_GameController;
typedef struct _SDL_GameController SDL_GameController;
union SDL_Event;

/*
	Clase con m�todos est�ticos.
	Gestiona la entrada en PC. 
	Toda la informaci�n se guarda en userInput
*/
class InputPC
{
public:
	/*
		Implementa la funci�n que ejecutar� cuando reciba un evento.
		Detecta el tipo de input y si le interesa, lo a�ade a InputPC
	*/
	class Observer : public InputObserver
	{
	public:
		virtual bool HandleEvent(const SDL_Event e) override;
	};

private:
	//Constants
	static const int JOYSTICK_MAX_VALUE;
	static const int JOYSTICK_DEAD_ZONE;
	static const int TRIGGER_MAX_VALUE;

	//Attributes
	static SDL_GameController* gameController;	//Manejador del GameController
	static Observer observer;					//Observa los eventos de Input de Platform. TODO: Puntero?????????
	static std::queue<SDL_Event> eventQueue;	//Cola con los eventos a procesar en el siguiente tick
	static UserInput userInput;					//Informacion del estado actual del input

public:

	/*
		Inicializa el input.
		Se registra a platform para escuchar el input.
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
		A�ade un evento a la cola de input para que se procese
	*/
	static void AddEvent(const SDL_Event e);

	/*
		Restringe un valor float al rango
	*/
	static float Clamp(float min, float max, float value);

};


