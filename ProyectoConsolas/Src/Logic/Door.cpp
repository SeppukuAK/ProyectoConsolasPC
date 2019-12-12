#include "Door.h"
#include "../Renderer/Image.h"
#include "Sprite.h"
#include "../Renderer/RendererThread.h"

const int Door::DOOR_ANIM_FRAMES = 4;
const float Door::ANIM_RATE = 1.0f;
Sprite** Door::spritesArray = nullptr;

void Door::Init(Image* image)
{
	spritesArray = new Sprite * [DOOR_ANIM_FRAMES];

	int spriteWidth = image->GetWidth() / DOOR_ANIM_FRAMES;
	int sy1 = 0;
	int sy2 = image->GetHeight();

	for (int i = 0; i < DOOR_ANIM_FRAMES; i++)
	{
		int sx1 = i * spriteWidth;
		int sx2 = spriteWidth * (i + 1);

		spritesArray[i] = new Sprite(image, sx1, sy1, sx2, sy2);
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
