#pragma once
#include <thread>
#include <atomic> 
/*
	Cuando se crea el objeto no se lanza automaticamente la hebra
*/
class RendererThread
{
private:
	std::thread* t;
	std::atomic <bool> quitRequested;	//Escritura y lectura seguras

public:
	/*
		Inicia la hebra
	*/
	void Start();

	/*
		Para la hebra
		TODO: Se le llama al acabar el bucle principal
	*/
	void Stop();
	void RenderLoop();
};

