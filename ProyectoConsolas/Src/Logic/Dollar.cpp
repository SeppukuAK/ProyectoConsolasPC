#include "Dollar.h"
#include "../Renderer/Image.h"
#include "../Utilities/Rect.h"
#include "Sprite.h"

#include <iostream>

const float Dollar::ANIM_RATE = 0.2f;
const int Dollar::NUM_SPRITES = 6;

Sprite** Dollar::dollarSprites = nullptr;

Dollar::Dollar(int x, int y) : Entity(x,y)
{
	sprites = dollarSprites;

	animTimer = 0.0f;
	_visible = false;
	_moneyReceived = false;
}

void Dollar::Init(Image* dollarImage)
{
	//Fondo dolar
	Rect sRect;
	sRect.Width = dollarImage->GetWidth() / NUM_SPRITES;
	sRect.Height = dollarImage->GetHeight();
	sRect.X = 0;
	sRect.Y = 0;

	dollarSprites = new Sprite * [NUM_SPRITES];

	for (int i = 0; i < NUM_SPRITES; i++)
	{
		sRect.X = i * sRect.Width;
		dollarSprites[i] = new Sprite(dollarImage, sRect);
	}
}

void Dollar::Release()
{
	for (int i = 0; i < NUM_SPRITES; ++i)
		delete[] dollarSprites[i];

	delete[] dollarSprites;
	dollarSprites = nullptr;
}

void Dollar::Update(float delta,float deltaTime)
{
	int newDollarState;
	if (_visible && _moneyReceived)
	{
		//Fin de la animación
		if (animTimer >= ANIM_RATE * 2)
			newDollarState = DollarState::DOLLAR_VISIBLE_MONEY;

		else
		{
			animTimer += deltaTime;

			if (animTimer >= ANIM_RATE)
				newDollarState = DollarState::DOLLAR_ANIM_1;
			else
				newDollarState = DollarState::DOLLAR_ANIM_0;

		}
	}
	else if (_visible)
		newDollarState = DollarState::DOLLAR_VISIBLE;

	else if (_moneyReceived)
		newDollarState = DollarState::DOLLAR_MONEY;

	else
		newDollarState = DollarState::DOLLAR_EMPTY;

	CheckState(delta, newDollarState);
}


