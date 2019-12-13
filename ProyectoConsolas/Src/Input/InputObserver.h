#pragma once

union SDL_Event;

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
