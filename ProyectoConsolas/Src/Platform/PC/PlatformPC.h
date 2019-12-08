#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h> //Utiliza la librería SDL: permite la carga de pngs y el pintado de píxeles
#include <string>
#include "../../Renderer/Utilities/Color.h"
#include "../../Renderer/Image.h"

/*
	Clase con métodos estáticos. Inicializa atributos de la plataforma.
	Gestiona la ventana. Detecta cerrar la ventana.
*/
class PlatformPC
{
private:
	static SDL_Window* window;		//Referencia a la ventana de SDL
	static std::string mediaPath;	//Ruta donde se encuentran los recursos

public:

	/*
		Inicializa SDL y crea la ventana con el modo y las dimensiones pasadas
	*/
	static void Init(int screenWidth, int screenHeight, int numBuffers);

	/*
		Destruye la ventana y cierra SDL
	*/
	static void Release();

	/*
		Devuelve un bool que indica si el usuario ha decidido terminar la aplicación.
		Devuelve true si se ha pulsado la 'X', cualquier tecla o el ratón
	*/
	static bool Tick();

	/*
		Carga una imagen en formato crudo y lo guarda en un array de color
	*/
	static Image * LoadImage(std::string path);

	//Getters
	static SDL_Window* GetWindow() { return window; };

	/*
		Devuelve la ruta donde se encuentran los recursos
	*/
	static std::string GetMediaPath() { return mediaPath; };
};

