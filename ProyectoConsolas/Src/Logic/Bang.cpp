#include "Bang.h"
#include "../Renderer/Image.h"
#include "../Utilities/Rect.h"
#include "Sprite.h"
#include <iostream>

const int Bang::NUM_SPRITES = 6;

Sprite** Bang::bangSprites = nullptr;

void Bang::Init(Image* bangImage)
{
	//Fondo dolar
	bangSprites = new Sprite *[NUM_SPRITES];

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

Bang::Bang(int x, int y, float duration) : Entity(x, y), _duration(duration)
{
	sprites = bangSprites;

	startAnimTime = 0.0f;
}

void Bang::Update(float delta, float time)
{
	int newDollarState;

	//No se ha calculado el tiempo final de la animación

	if (startAnimTime == 0.0f)
		startAnimTime = time;

	newDollarState = ((time - startAnimTime) * NUM_SPRITES ) / _duration;

	CheckState(delta, newDollarState);
}
