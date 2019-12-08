

//Se envuelve todo para evitar que se compile si no estamos en la plataforma
#if PLATFORM_PC

#include "InputPC.h"
#include <stdio.h> 
#include "../../Platform/Platform.h"

//Inicialización de atributos estáticos
SDL_Joystick* InputPC::gameController = NULL;


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
		gameController = SDL_JoystickOpen(0);
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
	SDL_JoystickClose(gameController);
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

			//GATILLOS
			//Los gatillos analogicos se mapean a los ejes 2 y 5 en mandos de xbox (y 3 y 4 en caso de los mandos de PS4.)
			//JOYSTICK IZQUIERDO
		case SDL_JOYAXISMOTION:
		{
			//Mando 0
			if (e.jaxis.which == 0)
			{
				//X axis motion 
				if (e.jaxis.axis == 0)
				{
					//Left of dead zone
					if (e.jaxis.value < 8000)// TODO: JOYSTICK_DEAD_ZONE
					{
						userInput.LeftJoystickHor = -1;
					}
					//Right of dead zone
					else if (e.jaxis.value > 8000)// TODO: JOYSTICK_DEAD_ZONE
					{
						userInput.LeftJoystickHor = 1;
					}
					else
					{
						userInput.LeftJoystickHor = 0;
					}
				}
				//Y axis motion
				else if (e.jaxis.axis == 1)
				{
					//Below of dead zone
					if (e.jaxis.value < 8000)
					{
						userInput.LeftJoystickVer = -1;
					}
					//Above of dead zone
					else if (e.jaxis.value > 8000)
					{
						userInput.LeftJoystickVer = 1;
					}
					else
					{
						userInput.LeftJoystickVer = 0;
					}
				}
			}
		default:
			break;

		}


		}
	}



#endif