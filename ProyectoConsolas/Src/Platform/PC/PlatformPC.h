#pragma once

#include <string>
#include <vector>		//TODO: Vector, habrá problemas?

//Declaración adelantada para evitar inclusión adicional
class InputObserver;
class Image;
union SDL_Event;

/*
	Clase con métodos estáticos. Inicializa atributos de la plataforma.
	Gestiona la ventana. Detecta cerrar la ventana.
	Proporciona facilidades para que otros se registren para recibir eventos. Observer
*/
class PlatformPC
{
private:
	static const std::string mediaPath;					//Ruta donde se encuentran los recursos
	static std::vector<InputObserver*> observers;		//Observadores del input

public:

	/*
		Inicializa SDL 
	*/
	static void Init();

	/*
		Cierra SDL
	*/
	static void Release();

	/*
		Procesa el input.
		Devuelve un bool que indica si el usuario ha decidido terminar la aplicación.
		-Devuelve true si se ha pulsado la 'X', la tecla escape o el ratón.
		-Cualquier otro evento informa a los observers.
	*/
	static bool Tick();

	/*
		Carga una imagen en formato crudo con cabecera.
		Devuelve una Image.
	*/
	static Image* LoadImage(std::string path);

	/*
		Devuelve la ruta donde se encuentran los recursos
	*/
	static std::string GetMediaPath() { return mediaPath; };

	/**
	 * Register an observer
	 * @param observer the observer object to be registered
	 */
	static void RegisterObserver(InputObserver* observer);

	/**
	 * Unregister an observer
	 * @param observer the observer object to be unregistered
	 */
	static void RemoveObserver(InputObserver* observer);


private:

	/*
		Notifica a todos los observers el evento hasta que uno lo consuma
	*/
	static void NotifyObservers(SDL_Event e);
};

