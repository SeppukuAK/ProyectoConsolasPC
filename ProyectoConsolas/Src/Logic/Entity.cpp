#include "Entity.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/RendererThread.h"
#include <iostream>

Entity::Entity(int x, int y) : _x(x), _y(y)
{
	sprites = nullptr;
	changedState = true;
	currentState = 0;
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
		changedState = lastFrameStateChanged + Renderer::GetNumBuffers() > delta;

}

void Entity::Render()
{
	if (changedState)
	{
		RenderCommand drawCommand;
		drawCommand.Type = RendererCommandType::DRAW_SPRITE;
		drawCommand.Param.DrawSpriteParams.PosX = _x;
		drawCommand.Param.DrawSpriteParams.PosY = _y;
		drawCommand.Param.DrawSpriteParams.Sprite = sprites[currentState];
		RendererThread::EnqueueCommand(drawCommand);
	}
}