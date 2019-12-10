

//Se envuelve todo para evitar que se compile si no estamos en la plataforma
#if PLATFORM_PC

#include "InputPC.h"
#include <stdio.h> 
#include "../../Platform/Platform.h"
#include <iostream>

//Inicialización de atributos estáticos
SDL_GameController* InputPC::gameController = NULL;
const int JOYSTICK_DEAD_ZONE = 8000; //TODO: ESTE ES EL DEADZONE PARA LOS STICKS, CUAL ES EL DEADZONE PARA LOS TRIGGERS?


void InputPC::Init()
{

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	//Comprobamos si hay joysticks conectados
	if (SDL_NumJoysticks() < 1)
	{
		printf("Warning: No joysticks connected!\n");
	}
	else
	{
		//Abrimos el joystick
		gameController = SDL_GameControllerOpen(0);
		if (gameController == NULL)
		{
			printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
		}
	}
}

void InputPC::Release()
{
	//Destruye el input
	//Close game  controller
	SDL_GameControllerClose(gameController);
	gameController = NULL;
}

void InputPC::Tick()
{
	SDL_Event e;		//Manejador de eventos
	bool quit = false;

	//Procesa los eventos de la cola
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
			//BOTONES
		case SDL_CONTROLLER_BUTTON_A:
			userInput.Cross = true;
			break;
		case SDL_CONTROLLER_BUTTON_B:
			userInput.Circle = true;
			break;
		case SDL_CONTROLLER_BUTTON_X:
			userInput.Square = true;
			break;
		case SDL_CONTROLLER_BUTTON_Y:
			userInput.Triangle = true;
			break;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			userInput.L1 = true;
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			userInput.R1 = true;
			break;

			//Eventos que utilizan ejes ( TRIGGERS Y STICKERS ) 
		case SDL_CONTROLLERAXISMOTION:
		{
			//Detecto si estoy usando el mando 0
			if (e.caxis.which == 0)
			{
				switch (e.caxis.axis)
				{
					//JOYSTICK IZQUIERDO
					
					//X axis motion LEFT AND RIGHT
				case SDL_CONTROLLER_AXIS_LEFTX:
				{
					//Left of dead zone
					if (e.caxis.value < 8000)// TODO: JOYSTICK_DEAD_ZONE
					{
						userInput.LeftJoystickHor = -1;
					}
					//Right of dead zone
					else if (e.caxis.value > 8000)// TODO: JOYSTICK_DEAD_ZONE
					{
						userInput.LeftJoystickHor = 1;
					}
					else
					{
						userInput.LeftJoystickHor = 0;
					}
					break;
				}
				//Y axis motion UP AND DOWN
				case SDL_CONTROLLER_AXIS_LEFTY:
				{
					//Below of dead zone
					if (e.caxis.value < 8000)
					{
						userInput.LeftJoystickVer = -1;
					}
					//Above of dead zone
					else if (e.caxis.value > 8000)
					{
						userInput.LeftJoystickVer = 1;
					}
					else
					{
						userInput.LeftJoystickVer = 0;
					}
					break;
				}

				//GATILLOS

				//Los gatillos analogicos se mapean a los ejes 2 y 5 en mandos de xbox (y 3 y 4 en caso de los mandos de PS4.) ????
				//TODO: No es una metralleta. Para que deje de ser disparo los 2 gatillos tienen que estar sueltos. Hay que convertirlos de analogico a digital.
				//TODO: CREO QUE ESTA MAL
				case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
					//Below of dead zone
					if (e.caxis.value < 8000)
					{
						userInput.L2 = -1;
					}
					//Above of dead zone
					else if (e.caxis.value > 8000)
					{
						userInput.L2 = 1;
					}
					else
					{
						userInput.L2 = 0;
					}
					break;

				case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
					//Below of dead zone
					if (e.caxis.value < 8000)
					{
						userInput.R2 = -1;
					}
					//Above of dead zone
					else if (e.caxis.value > 8000)
					{
						userInput.R2 = 1;
					}
					else
					{
						userInput.R2 = 0;
					}
					break;

				}
			}
			break;
		}

		//Evento en el cual se detecta que se ha añadido un mando nuevo
		case SDL_CONTROLLERDEVICEADDED:
			std::cout << "Se ha conectado el mando" << e.caxis.which << std::endl;
			break;
			//Evento en el cual se detecta que se ha desconectado un mando 
		case SDL_CONTROLLERDEVICEREMOVED:
			std::cout << "Se ha desconectado el mando" << e.caxis.which << std::endl;
			break;

		default:
			break;

		}
	}
}



#endif