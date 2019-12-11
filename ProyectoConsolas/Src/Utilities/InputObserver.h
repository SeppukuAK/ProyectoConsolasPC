#pragma once

#include <SDL.h>

/**
 * Interface for the Observer
 */
class InputObserver
{
public:
	/*
		Update the state of this observer
	*/
	virtual bool HandleEvent(SDL_Event e) = 0;
};

