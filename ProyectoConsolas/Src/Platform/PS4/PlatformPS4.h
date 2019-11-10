#pragma once

/*
	Clase con métodos estáticos. Inicializa atributos de la plataforma.
	Inicializa memoria y nucleo
*/
class PlatformPS4
{
	/*
		Inicializa memoria y nucleo
	*/
	static void Init();

	/*
		Libera recursos
	*/
	static void Release();

	/*
		Devuelve un bool que indica si el usuario ha decidido terminar la aplicación.
		En PS4 no se detecta
	*/
	static bool Tick() { return true; };
};

