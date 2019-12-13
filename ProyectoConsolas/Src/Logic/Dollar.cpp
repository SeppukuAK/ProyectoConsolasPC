#include "Dollar.h"
#include "../Renderer/Image.h"
#include "../Utilities/Rect.h"
#include "Sprite.h"
#include "../Renderer/RendererThread.h"

const int Dollar::NUM_SPRITES = 6;

Sprite** Dollar::dollarSprites = nullptr;

void Dollar::Init(Image* dollarImage)
{
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

Dollar::Dollar(int x, int y) : _x(x), _y(y)
{
	state = DollarState::DOLLAR_EMPTY;
}

void Dollar::Update(DollarState newState)
{
	state = newState;
}

void Dollar::Render()
{
	RenderCommand renderCommand;
	renderCommand.Type = RendererCommandType::DRAW_SPRITE;
	renderCommand.Param.DrawSpriteParams.PosX = _x;
	renderCommand.Param.DrawSpriteParams.PosY = _y;
	renderCommand.Param.DrawSpriteParams.Sprite = dollarSprites[state];

	RendererThread::EnqueueCommand(renderCommand);
}
