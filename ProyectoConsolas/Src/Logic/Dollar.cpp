#include "Dollar.h"
#include "../Renderer/Image.h"
#include "../Utilities/Rect.h"
#include "Sprite.h"
#include "../Renderer/RendererThread.h"

const float Dollar::ANIM_RATE = 5.0f;
const int Dollar::NUM_SPRITES = 6;
const int Dollar::ANIM_FRAMES = 2;

Sprite** Dollar::dollarSprites = nullptr;
Sprite** Dollar::dollarAnimSprites = nullptr;

Dollar::Dollar(int x, int y) : _x(x), _y(y)
{
	frameIndex = 0;
	animTimer = 0.0f;
//	state = DollarState::DOLLAR_EMPTY;
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

	//Anim dollars

	dollarAnimSprites = new Sprite * [ANIM_FRAMES];


	for (int i = 0; i < ANIM_FRAMES; i++)
	{
		dollarAnimSprites[i] = dollarSprites[DOLLAR_ANIM_0+i];
	}

}

void Dollar::Release()
{
	for (int i = 0; i < ANIM_FRAMES; ++i)
		delete[] dollarAnimSprites[i];

	delete[] dollarAnimSprites;
	dollarAnimSprites = nullptr;


	for (int i = 0; i < NUM_SPRITES; ++i)
		delete[] dollarSprites[i];

	delete[] dollarSprites;
	dollarSprites = nullptr;
}



void Dollar::Update(DollarState newState, float deltaTime)
{
	state = newState;
	animTimer += deltaTime;

	//Se ha ganado un dolar
	if (state == DOLLAR_WIN) {
		if (animTimer >= ANIM_RATE)
		{
			frameIndex = (frameIndex + 1) % ANIM_FRAMES;
			animTimer = 0.0f;
		}
	}
}

void Dollar::Render()
{
	drawDollarCommand.Type = RendererCommandType::DRAW_SPRITE;
	drawDollarCommand.Param.DrawSpriteParams.PosX = _x;
	drawDollarCommand.Param.DrawSpriteParams.PosY = _y;
	drawDollarCommand.Param.DrawSpriteParams.Sprite = dollarSprites[frameIndex];
	RendererThread::EnqueueCommand(drawDollarCommand);

	//Anim
	drawDollarAnimCommand.Type = RendererCommandType::DRAW_SPRITE;
	drawDollarAnimCommand.Param.DrawSpriteParams.PosX = _x;
	drawDollarAnimCommand.Param.DrawSpriteParams.PosY = _y;
	drawDollarAnimCommand.Param.DrawSpriteParams.Sprite = dollarAnimSprites[frameIndex];
	RendererThread::EnqueueCommand(drawDollarAnimCommand);
}
