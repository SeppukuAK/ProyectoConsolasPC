#include "Entity.h"
#include <iostream>
#include "../Renderer/Renderer.h"
#include "Sprite.h"

Entity::Entity(int x, int y) : _x(x), _y(y)
{
	sprites = nullptr;
	changedState = false;
	currentState = -1;
	lastFrameStateChanged = 0;
}

void Entity::Reset()
{
	changedState = false;
	currentState = -1;
	lastFrameStateChanged = 0;
}

void Entity::CheckState(float delta, int newState)
{
	if (currentState != newState)
	{
		lastFrameStateChanged = delta;
		currentState = newState;
		changedState = true;
	}
	else
		changedState = (lastFrameStateChanged + Renderer::GetNumBuffers()) > delta;

}

void Entity::Update(float delta, float time)
{
	CheckState(delta, 0);
}

void Entity::Render()
{
	if (changedState)
		sprites[currentState]->Draw(_x,_y);
}

