#include "Sprite.h"
#include "../Renderer/RendererThread.h"
#include <iostream>

using namespace std;

Sprite::Sprite(Image* image, Rect sRect) : _image(image), _sRect(sRect)
{
}

void Sprite::Draw(int posX, int posY) {
	RenderCommand drawCommand;
	drawCommand.Type = RendererCommandType::DRAW_SPRITE;
	drawCommand.Param.DrawSpriteParams.PosX = posX;
	drawCommand.Param.DrawSpriteParams.PosY = posY;
	drawCommand.Param.DrawSpriteParams.Image = _image;
	drawCommand.Param.DrawSpriteParams.SourceRect = _sRect;
	RendererThread::EnqueueCommand(drawCommand);
}


void Sprite::DrawPartially(int posX, int posY, Rect partialRect)
{
	RenderCommand drawCommand;
	drawCommand.Type = RendererCommandType::DRAW_SPRITE;
	drawCommand.Param.DrawSpriteParams.PosX = posX;
	drawCommand.Param.DrawSpriteParams.PosY = posY;
	drawCommand.Param.DrawSpriteParams.Image = _image;
	drawCommand.Param.DrawSpriteParams.SourceRect = partialRect;
	RendererThread::EnqueueCommand(drawCommand);

}



