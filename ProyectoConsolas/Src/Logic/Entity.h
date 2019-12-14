#pragma once

class Sprite;

class Entity
{
private:
	int _x, _y;
	bool changedState;
	int lastFrameStateChanged;		//Ultimo tick en el que ha habido un cambio de estado

protected:
	Sprite** sprites;
	int currentState;

public:
	Entity(int x, int y);
	virtual void Update(float delta, float deltaTime) = 0;
	void Render();

protected:
	void CheckState(float delta, int newState);

};

