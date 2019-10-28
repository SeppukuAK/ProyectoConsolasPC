#pragma once

/*
	Inicializa atributos de la plataforma.
	Inicializa memoria y nucleo
*/
class PlatformPS4
{
	//TODO:Poner atributos
	//TODO: Los métodos son estáticos???
	static void Init();
	static void Release();	//Libera recursos
	/*
	Devuelve un bool que indica si el usuario ha decidido terminar la aplicación.
	En PS4 no se detecta
	TODO: Se le llamará en el bucle principal
	*/
	static bool Tick() { return false; };
};

