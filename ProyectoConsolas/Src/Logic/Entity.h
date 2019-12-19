#pragma once

class Sprite;

class Entity
{
protected:
	int _x, _y;
	int lastFrameStateChanged;		//Ultimo tick en el que ha habido un cambio de estado
	Sprite** sprites;
	int currentState;
	bool changedState;

public:
	Entity(int x, int y);
	virtual void Reset();

	virtual void Update(float delta, float time);
	virtual void Render();

protected:
	void CheckState(float delta, int newState);

};

