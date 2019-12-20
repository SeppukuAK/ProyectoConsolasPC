#include "Time.h"

//Declaración de variables estáticas
double Time::time = 0.0;
double Time::deltaTime = 0.0;
std::chrono::time_point<std::chrono::system_clock> Time::initTime;
double Time::previousTime = 0.0;

void Time::Init()
{
	//Obtención del tiempo inicial de la aplicación
	initTime = std::chrono::system_clock::now();
}

void Time::Tick()
{
	//Tiempo actual
	std::chrono::time_point<std::chrono::system_clock> frameTime = std::chrono::system_clock::now();		

	//Actualización tiempo transcurrido
	std::chrono::duration<double> elapsed_seconds = frameTime - initTime;									
	time = elapsed_seconds.count();
	deltaTime = time - previousTime;
	previousTime = time;
}