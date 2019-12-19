#include "FrameDoor.h"
#include "../Renderer/Image.h"
#include "../Utilities/Rect.h"
#include "Sprite.h"
#include <iostream>

Sprite* FrameDoor::frameDoorSprite = nullptr;
Rect FrameDoor::scrollRect;

FrameDoor::FrameDoor(int x, int y) : Entity(x, y)
{
	sprites = &frameDoorSprite;
}

void FrameDoor::Init(Image* frameDoorImage)
{
	//Fondo dolar
	scrollRect.Width = frameDoorImage->GetWidth();
	scrollRect.Height = frameDoorImage->GetHeight();
	scrollRect.X = 0;
	scrollRect.Y = 0;

	frameDoorSprite = new Sprite(frameDoorImage, scrollRect);
}

void FrameDoor::Release()
{
	delete frameDoorSprite;
	frameDoorSprite = nullptr;
}

void FrameDoor::RenderWithDelta(int posX) {
	Rect rLeft = scrollRect;
	rLeft.X = scrollRect.Width - posX;
	rLeft.Width = posX;


	Rect rRight = scrollRect;
	rRight.X = 0;
	rRight.Width = scrollRect.Width - posX;

	sprites[currentState]->DrawPartially(_x + posX, _y, rRight);
	sprites[currentState]->DrawPartially(_x, _y, rLeft);



}