
//Se envuelve todo para evitar que se compile si no estamos en la plataforma
#if PLATFORM_PC

#include "InputPC.h"
#include <stdio.h> 
#include "../../Platform/PC/PlatformPC.h"
#include <SDL.h>		/* SDL. Pintado */
#include <stdlib.h>     /* abs */
#include <iostream>
using namespace std;

//Inicialización de atributos estáticos
//-32768 and 32767
const int InputPC::JOYSTICK_MAX_VALUE = 32767;
const int InputPC::JOYSTICK_DEAD_ZONE = 8000;
const int InputPC::TRIGGER_MAX_VALUE = 32767;

SDL_GameController* InputPC::gameController = NULL;
InputPC::Observer InputPC::observer = InputPC::Observer();
std::queue<SDL_Event> InputPC::eventQueue = std::queue<SDL_Event>();
UserInput InputPC::userInput = UserInput();

bool InputPC::Observer::HandleEvent(const SDL_Event e)
{
	bool eventConsumed = false;
	switch (e.type)
	{
	case SDL_KEYDOWN:
	case SDL_KEYUP:
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_1:
		case SDLK_2:
		case SDLK_3:
		case SDLK_o:
		case SDLK_p:
		{
			eventConsumed = true;
			break;
		}
		}
		break;
	}

	case SDL_CONTROLLERBUTTONDOWN:
	case SDL_CONTROLLERBUTTONUP:
	{
		switch (e.cbutton.button)
		{
			//case SDL_CONTROLLER_BUTTON_A:
			//case SDL_CONTROLLER_BUTTON_B:
			//case SDL_CONTROLLER_BUTTON_X:
			//case SDL_CONTROLLER_BUTTON_Y:
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
		{
			eventConsumed = true;
			break;
		}
		}
		break;
	}

	case SDL_CONTROLLERAXISMOTION:
	{
		if (e.caxis.which == 0)
		{
			switch (e.caxis.axis)
			{
			case SDL_CONTROLLER_AXIS_LEFTX:
				//case SDL_CONTROLLER_AXIS_LEFTY:
			case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
			case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
			{
				eventConsumed = true;
				break;
			}
			}

		}
		break;
	}
	}

	if (eventConsumed)
		InputPC::AddEvent(e);

	return eventConsumed;
}

void InputPC::Init()
{
	//TODO: Que hacemos con esto?
	////Set texture filtering to linear
	//if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	//{
	//	printf("Warning: Linear texture filtering not enabled!");
	//}

	//Comprobamos si hay joysticks conectados
	if (SDL_NumJoysticks() < 1)
	{
		printf("Warning: No joysticks connected!\n");
	}
	else
	{
		// Se abre el primer mando conectado. Ahora genera eventos
		gameController = SDL_GameControllerOpen(0);
		if (gameController == NULL)
		{
			printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
		}
	}

	//La clase observa los eventos
	PlatformPC::RegisterObserver(&observer);
}

void InputPC::Release()
{
	PlatformPC::RemoveObserver(&observer);

	//Close game  controller
	SDL_GameControllerClose(gameController);
	gameController = NULL;
}

void InputPC::Tick()
{
	//Procesa los eventos de la cola
	while (!eventQueue.empty())
	{
		SDL_Event e = eventQueue.front();
		eventQueue.pop();

		switch (e.type)
		{
		case SDL_KEYDOWN:
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_1:
			{
				userInput.Key_1 = true;
				break;
			}
			case SDLK_2:
			{
				userInput.Key_2 = true;
				break;
			}
			case SDLK_3:
			{
				userInput.Key_3 = true;
				break;
			}
			case SDLK_o:
			{
				userInput.Key_O = true;
				break;
			}
			case SDLK_p:
			{
				userInput.Key_P = true;
				break;
			}
			}
			break;
		}
		case SDL_KEYUP:
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_1:
			{
				userInput.Key_1 = false;
				break;
			}
			case SDLK_2:
			{
				userInput.Key_2 = false;
				break;
			}
			case SDLK_3:
			{
				userInput.Key_3 = false;
				break;
			}
			case SDLK_o:
			{
				userInput.Key_O = false;
				break;
			}
			case SDLK_p:
			{
				userInput.Key_P = false;
				break;
			}
			}
			break;
		}

		case SDL_CONTROLLERBUTTONDOWN:
		{
			switch (e.cbutton.button)
			{
				//case SDL_CONTROLLER_BUTTON_A:
				//	userInput.Cross = true;
				//	break;
				//case SDL_CONTROLLER_BUTTON_B:
				//	userInput.Circle = true;
				//	break;
				//case SDL_CONTROLLER_BUTTON_X:
				//	userInput.Square = true;
				//	break;
				//case SDL_CONTROLLER_BUTTON_Y:
				//	userInput.Triangle = true;
				//	break;
			case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			{
				userInput.L1 = true;
				break;
			}
			case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			{
				userInput.R1 = true;
				break;
			}
			}
			break;
		}

		case SDL_CONTROLLERBUTTONUP:
		{
			switch (e.cbutton.button)
			{
				//case SDL_CONTROLLER_BUTTON_A:
				//	userInput.Cross = false;
				//	break;
				//case SDL_CONTROLLER_BUTTON_B:
				//	userInput.Circle = false;
				//	break;
				//case SDL_CONTROLLER_BUTTON_X:
				//	userInput.Square = false;
				//	break;
				//case SDL_CONTROLLER_BUTTON_Y:
				//	userInput.Triangle = false;
				//	break;
			case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			{
				userInput.L1 = false;
				break;
			}
			case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			{
				userInput.R1 = false;
				break;
			}
			}
			break;
		}
		//Eventos que utilizan ejes ( TRIGGERS Y STICKERS ) 
		case SDL_CONTROLLERAXISMOTION:
		{
			//Mando 0
			if (e.caxis.which == 0)
			{
				switch (e.caxis.axis)
				{
					//Joystick
				case SDL_CONTROLLER_AXIS_LEFTX:
				{
					//Aplicación de deadzone
					if (abs(e.caxis.value) < JOYSTICK_DEAD_ZONE)
						userInput.HorizontalAxis = 0;
					else
						userInput.HorizontalAxis = Clamp(-1.0f, 1.0f, (float)e.caxis.value / JOYSTICK_MAX_VALUE);

					break;
				}

				//case SDL_CONTROLLER_AXIS_LEFTY:
				//{
				//	//Aplicación de deadzone
				//	if (abs(e.caxis.value) < JOYSTICK_DEAD_ZONE)
				//		userInput.VerticalAxis = 0;
				//	else
				//		userInput.VerticalAxis = Clamp(-1.0f, 1.0f, (float)e.caxis.value / JOYSTICK_MAX_VALUE);

				//	break;
				//}

					//GATILLOS
				case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
				{
					//(0-TRIGGER_MAX_VALUE) --> (0-TRIGGER_MAX_VALUE) --> [-1,+1]
					//Convertimos el valor que tiene rango [0,TRIGGER_MAX_VALUE] al rango [-1,1]
					userInput.L2 = (float)(2 * e.caxis.value - TRIGGER_MAX_VALUE) / TRIGGER_MAX_VALUE;

					break;
				}
				case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
				{
					//(0-TRIGGER_MAX_VALUE) --> (0-TRIGGER_MAX_VALUE) --> [-1,+1]
					//Convertimos el valor que tiene rango [0,TRIGGER_MAX_VALUE] al rango [-1,1]
					userInput.R2 = (float)(2 * e.caxis.value - TRIGGER_MAX_VALUE) / TRIGGER_MAX_VALUE;
					break;
				}
				}
			}
			break;
		}

		default:
			break;

		}
	}
}

void InputPC::AddEvent(const SDL_Event e)
{
	eventQueue.push(e);
}

float InputPC::Clamp(float min, float max, float value)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}

#endif