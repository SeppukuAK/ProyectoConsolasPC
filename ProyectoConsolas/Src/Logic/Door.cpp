#include "Door.h"
#include "../Renderer/Image.h"
#include "Sprite.h"
#include "../Utilities/Rect.h"

const float Door::ANIM_RATE = 5.0f;

int Door::_doorAnimFrames = 0;
Sprite* Door::doorFrameSprite = nullptr;
Sprite** Door::doorSprites = nullptr;

void Door::Init(Image* doorFrameImage, Image* doorsImage, int doorAnimFrames)
{
	_doorAnimFrames = doorAnimFrames;

	//Creación sprite DoorFrame
	Rect sRect;
	sRect.Width = doorFrameImage->GetWidth();
	sRect.Height = doorFrameImage->GetHeight();
	sRect.X = 0;
	sRect.Y = 0;

	doorFrameSprite = new Sprite(doorFrameImage, sRect);

	//Creación sprites DoorsAnims
	doorSprites = new Sprite * [_doorAnimFrames];

	sRect.Width = doorsImage->GetWidth() / _doorAnimFrames;
	sRect.Height = doorsImage->GetHeight();
	sRect.Y = 0;

	for (int i = 0; i < _doorAnimFrames; i++)
	{
		sRect.X = i * sRect.Width;
		doorSprites[i] = new Sprite(doorsImage, sRect);
	}
}

void Door::Release()
{
	delete doorFrameSprite;
	doorFrameSprite = nullptr;

	for (int i = 0; i < _doorAnimFrames; ++i)
		delete[] doorSprites[i];

	delete[] doorSprites;
	doorSprites = nullptr;
}

Door::Door(int x, int y) : _x(x), _y(y)
{
	frameIndex = 0;
	animTimer = 0.0f;

	//Creación de los comandos
	//Marco de la puerta
	drawDoorFrameCommand.Type = RendererCommandType::DRAW_SPRITE;
	drawDoorFrameCommand.Param.DrawSpriteParams.Sprite = doorFrameSprite;
	drawDoorFrameCommand.Param.DrawSpriteParams.PosX = _x;
	drawDoorFrameCommand.Param.DrawSpriteParams.PosY = _y;

	//Puerta 
	drawDoorCommand.Type = RendererCommandType::DRAW_SPRITE;
	drawDoorCommand.Param.DrawSpriteParams.Sprite = doorSprites[frameIndex];
	drawDoorCommand.Param.DrawSpriteParams.PosX = _x+32;
	drawDoorCommand.Param.DrawSpriteParams.PosY = _y+24;
}

Door::~Door()
{
}

void Door::Update(float deltaTime) {
	animTimer += deltaTime;

	if (animTimer >= ANIM_RATE)
	{
		frameIndex = (frameIndex + 1) % _doorAnimFrames;
		drawDoorCommand.Param.DrawSpriteParams.Sprite = doorSprites[frameIndex];
		animTimer = 0.0f;
	}
}

void Door::Render() {
	RendererThread::EnqueueCommand(drawDoorFrameCommand);

	RendererThread::EnqueueCommand(drawDoorCommand);
}
