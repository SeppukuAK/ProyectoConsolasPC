#include "Time.h"

double Time::time = 0.0;
double Time::deltaTime = 0.0;

std::chrono::time_point<std::chrono::system_clock> Time::initTime;
double Time::previous = 0.0;

void Time::Init()
{
	initTime = std::chrono::system_clock::now();
}

void Time::Tick()
{
	//GetCurrentTime
	std::chrono::time_point<std::chrono::system_clock> frameTime = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = frameTime - initTime;
	time = elapsed_seconds.count();
	deltaTime = time - previous;
	previous = time;
}