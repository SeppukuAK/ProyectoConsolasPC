#include "Dollar.h"
#include "../Renderer/Image.h"
#include "../Utilities/Rect.h"
#include "Sprite.h"

#include <iostream>

const float Dollar::ANIM_RATE = 0.4f;
const int Dollar::NUM_SPRITES = 6;

Sprite** Dollar::dollarSprites = nullptr;

Dollar::Dollar(int x, int y) : Entity(x, y)
{
	sprites = dollarSprites;

	endAnimTime = 0.0f;
	_visible = false;
	_moneyReceived = false;
	winingMoney = false;
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

void Dollar::Update(float delta, float time)
{
	int newDollarState;

	//Recibiendo ahora la moneda
	if (winingMoney)
	{
		switch (currentState)
		{
		case DollarState::DOLLAR_ANIM_0:
			if (time >= endAnimTime - ANIM_RATE / 2)
				newDollarState = DollarState::DOLLAR_ANIM_1;
			else
				newDollarState = DollarState::DOLLAR_ANIM_0;
			break;

		case DollarState::DOLLAR_ANIM_1:
			//Fin de la animación
			if (time >= endAnimTime)
			{
				winingMoney = false;
				newDollarState = DollarState::DOLLAR_VISIBLE_MONEY;
			}
			else
				newDollarState = DollarState::DOLLAR_ANIM_1;
			break;
		default:
			//Se inicia la animación
			_moneyReceived = true;
			endAnimTime = time + ANIM_RATE;
			newDollarState = DollarState::DOLLAR_ANIM_0;
			break;

		}
	}

	else if (_visible && _moneyReceived)
		newDollarState = DollarState::DOLLAR_VISIBLE_MONEY;

	else if (_visible)
		newDollarState = DollarState::DOLLAR_VISIBLE;

	else if (_moneyReceived)
		newDollarState = DollarState::DOLLAR_MONEY;

	else
		newDollarState = DollarState::DOLLAR_EMPTY;

	CheckState(delta, newDollarState);
}


