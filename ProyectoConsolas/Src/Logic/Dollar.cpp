#include "Dollar.h"
#include "../Renderer/Image.h"
#include "../Utilities/Rect.h"
#include "Sprite.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/RendererThread.h"
#include <iostream>

//const float Dollar::ANIM_RATE = 5.0f;
const int Dollar::NUM_SPRITES = 6;
//const int Dollar::ANIM_FRAMES = 2;

Sprite** Dollar::dollarSprites = nullptr;

Dollar::Dollar(int x, int y) : _x(x), _y(y)
{
	//frameIndex = 0;
	//animTimer = 0.0f;
	_visible = false;
	_moneyReceived = false;
	changedState = true;
	dollarState = DollarState::DOLLAR_EMPTY;
	lastFrameStateChanged = 0;
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
	DollarState newDollarState;
	if (_visible && _moneyReceived)
	{
		newDollarState = DollarState::DOLLAR_VISIBLE_MONEY;
	}
	else if (_visible)
	{
		newDollarState = DollarState::DOLLAR_VISIBLE;
	}
	else if (_moneyReceived)
	{
		newDollarState = DollarState::DOLLAR_MONEY;
	}
	else
	{
		newDollarState = DollarState::DOLLAR_EMPTY;
	}

	if (dollarState != newDollarState)
	{
		lastFrameStateChanged = delta;
		dollarState = newDollarState;
		changedState = true;

	}
	else
	{
		if (lastFrameStateChanged + Renderer::GetNumBuffers() > delta)
		{
			changedState = true;

		}
		else
			changedState = false;
	}

	//state = DOLLAR_WIN;
	//animTimer += deltaTime;

	////Se ha ganado un dolar
	//if (state == DOLLAR_WIN) {
	//	if (animTimer >= ANIM_RATE)
	//	{
	//		frameIndex = (frameIndex + 1) % ANIM_FRAMES;
	//		animTimer = 0.0f;
	//	}
	//}
}

void Dollar::Render()
{
	if (changedState)
	{
		std::cout << "Me pinto" << std::endl;

		RenderCommand drawDollarCommand;
		drawDollarCommand.Type = RendererCommandType::DRAW_SPRITE;
		drawDollarCommand.Param.DrawSpriteParams.PosX = _x;
		drawDollarCommand.Param.DrawSpriteParams.PosY = _y;
		drawDollarCommand.Param.DrawSpriteParams.Sprite = dollarSprites[dollarState];
		RendererThread::EnqueueCommand(drawDollarCommand);
	}
}
