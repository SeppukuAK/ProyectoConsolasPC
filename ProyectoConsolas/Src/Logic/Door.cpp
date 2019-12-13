#include "Door.h"
#include "../Renderer/Image.h"
#include "Sprite.h"
#include "../Renderer/RendererThread.h"
#include "../Utilities/Rect.h"

const int Door::DOOR_ANIM_FRAMES = 4;
const float Door::ANIM_RATE = 1.0f;
Sprite** Door::spritesArray = nullptr;

void Door::Init(Image* image)
{
	spritesArray = new Sprite * [DOOR_ANIM_FRAMES];

	Rect sRect;
	sRect.Width = image->GetWidth() / DOOR_ANIM_FRAMES;
	sRect.Height = image->GetHeight();
	sRect.Y = 0;

	for (int i = 0; i < DOOR_ANIM_FRAMES; i++)
	{
		sRect.X = i * sRect.Width;
		spritesArray[i] = new Sprite(image, sRect);
	}
}
void Door::Release()
{
	for (int i = 0; i < DOOR_ANIM_FRAMES; ++i)
		delete[] spritesArray[i];

	delete[] spritesArray;
	spritesArray = nullptr;
}


Door::Door()
{
	frameIndex = 0;
	animTimer = 0.0f;
}

Door::~Door()
{
}

void Door::Update(float deltaTime) {
	animTimer += deltaTime;

	if (animTimer >= ANIM_RATE)
	{
		frameIndex = (frameIndex + 1) % DOOR_ANIM_FRAMES;
		animTimer = 0.0f;
	}
}

void Door::Render() {
	RenderCommand drawSpriteCommand;
	drawSpriteCommand.Type = RendererCommandType::DRAW_SPRITE;
	drawSpriteCommand.Param.DrawSpriteParams.Sprite = spritesArray[frameIndex];
	drawSpriteCommand.Param.DrawSpriteParams.PosX = 100;
	drawSpriteCommand.Param.DrawSpriteParams.PosY = 100;

	RendererThread::EnqueueCommand(drawSpriteCommand);
}
