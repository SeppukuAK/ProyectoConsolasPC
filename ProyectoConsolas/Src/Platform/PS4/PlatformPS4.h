
/*
	Inicializa atributos de la plataforma.
	Inicializa memoria y nucleo
*/
class PlatformPS4
{
	//TODO:Poner atributos
	static void Init();
	static void Release();	//Libera recursos

	/*
	Devuelve un bool que indica si el usuario ha decidido terminar la aplicaci�n.
	En PS4 no se detecta
	TODO: Se le llamar� en el bucle principal
	*/
	static bool Tick() { return false; };
};

