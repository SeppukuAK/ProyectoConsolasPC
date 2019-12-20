#pragma once
#include <chrono>

/*
	Manejador del reloj del juego
*/
class Time
{
public:
	static double time;			//Tiempo transcurrido desde el inicio de la ejecución en este frame
	static double deltaTime;	//Tiempo transcurrido entre este frame y el anterior

private:
	static std::chrono::time_point<std::chrono::system_clock> initTime;			//Guarda el valor del tiempo al inicio de la ejecución
	static double previousTime;													//Tiempo transcurrido desde el inicio de la ejecución en el anterior frame

public:
	/*
		Inicializa el temporizador
	*/
	static void Init();

	/*
		Actualiza el valor de time y deltaTime
	*/
	static void Tick();
};

