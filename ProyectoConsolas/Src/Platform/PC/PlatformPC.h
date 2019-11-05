
/*
	Inicializa atributos de la plataforma.
	Gestiona la ventana. Detecta cerrar la ventana.
*/
class PlatformPC
{
	//TODO:Poner atributos
	static void Init();
	static void Release();	//Libera recursos

	/*
	Devuelve un bool que indica si el usuario ha decidido terminar la aplicación. 
	TODO: Gestionar la ventana comprobando si tiene un evento
	TODO: Se le llamará en el bucle principal
	*/
	static bool Tick();	 
};

