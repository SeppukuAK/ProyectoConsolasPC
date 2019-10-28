#pragma once

/*
	Inicializa atributos de la plataforma.
	Gestiona la ventana. Detecta cerrar la ventana.
*/
class PlatformPC
{
	//TODO:Poner atributos
	//TODO: Los métodos son estáticos???
	static void Init();
	static void Release();	//Libera recursos

	/*
	Devuelve un bool que indica si el usuario ha decidido terminar la aplicación. 
	Gestionar la ventana
	TODO: Se le llamará en el bucle principal
	*/
	static bool Tick();	 
};

