#include "Bang.h"
#include "../Renderer/Image.h"
#include "../Utilities/Rect.h"
#include "Sprite.h"
#include <iostream>
#include "WestBank.h"

const int Bang::NUM_SPRITES = 6;

Sprite** Bang::bangSprites = nullptr;

void Bang::Init(Image* bangImage)
{
	//Fondo dolar
	bangSprites = new Sprite * [NUM_SPRITES];

	Rect sRect;
	sRect.Width = bangImage->GetWidth() / NUM_SPRITES;
	sRect.Height = bangImage->GetHeight();
	sRect.X = 0;
	sRect.Y = 0;

	for (int i = 0; i < NUM_SPRITES; i++)
	{
		sRect.X = i * sRect.Width;
		bangSprites[i] = new Sprite(bangImage, sRect);
	}
}

void Bang::Release()
{
	for (int i = 0; i < NUM_SPRITES; ++i)
		delete bangSprites[i];

	delete[] bangSprites;
	bangSprites = nullptr;
}

Bang::Bang(int x, int y) : Entity(x, y)
{
	sprites = bangSprites;
	endAnimTime = 0.0f;
}

void Bang::Reset()
{
	Entity::Reset();
	endAnimTime = 0.0f;
}


void Bang::Update(float delta, float time)
{
	int newBangState = 0;

	if (endAnimTime == 0.0f)
		endAnimTime = time + WestBank::BANG_TIME;

	float leftTime = endAnimTime - time;
	if (leftTime >= 0)
	{
		float currentTime = WestBank::BANG_TIME - leftTime;
		newBangState = (currentTime * NUM_SPRITES) / WestBank::BANG_TIME;
	}

	CheckState(delta, newBangState);
}
