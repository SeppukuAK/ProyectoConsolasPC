#pragma once
#include <chrono>

class Time
{
public:
	static double time;
	static double deltaTime;

private:
	static std::chrono::time_point<std::chrono::system_clock> initTime;
	static double previous;

public:
	static void Init();
	static void Release();

	static void Tick();


};

