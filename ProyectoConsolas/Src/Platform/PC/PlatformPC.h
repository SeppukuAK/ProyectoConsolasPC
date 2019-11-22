#pragma once
#include <SDL.h> //Utiliza la librer�a SDL: permite la carga de pngs y el pintado de p�xeles

/*
	Clase con m�todos est�ticos. Inicializa atributos de la plataforma.
	Gestiona la ventana. Detecta cerrar la ventana.
*/
class PlatformPC
{
private:
	static SDL_Window* window;		//Referencia a la ventana de SDL

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
		Devuelve un bool que indica si el usuario ha decidido terminar la aplicaci�n.
		Devuelve true si se ha pulsado la 'X' o cualquier tecla
	*/
	static bool Tick();

	//Getters
	static SDL_Window* GetWindow() { return window; };
};

