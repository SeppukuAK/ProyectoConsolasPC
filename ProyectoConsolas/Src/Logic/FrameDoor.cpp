#include "FrameDoor.h"
#include "../Renderer/Image.h"
#include "../Utilities/Rect.h"
#include "Sprite.h"
#include <iostream>

Sprite* FrameDoor::frameDoorSprite = nullptr;

FrameDoor::FrameDoor(int x, int y) : Entity(x, y)
{
	sprites = &frameDoorSprite;
}

void FrameDoor::Init(Image* frameDoorImage)
{
	//Fondo dolar
	Rect sRect;
	sRect.Width = frameDoorImage->GetWidth();
	sRect.Height = frameDoorImage->GetHeight();
	sRect.X = 0;
	sRect.Y = 0;

	frameDoorSprite = new Sprite(frameDoorImage, sRect);
}

void FrameDoor::Release()
{
	delete frameDoorSprite;
	frameDoorSprite = nullptr;
}

void FrameDoor::Update(float delta, float time)
{	
	CheckState(delta, 0);
}


