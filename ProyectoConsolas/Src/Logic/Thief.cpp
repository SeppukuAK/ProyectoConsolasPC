#include "Thief.h"
#include "Door.h"
#include "../Renderer/Image.h"
#include "../Utilities/Rect.h"
#include "Sprite.h"
#include <iostream>

using namespace std;

const float Thief::ANIM_RATE = 1.0f;
const int Thief::NUM_SPRITES = 3;
const int Thief::TOTAL_SPRITES = 5;


Sprite** Thief::thiefSprites = nullptr;
int Thief::spriteWidth = 0;
int Thief::spriteHeight = 0;

Thief::Thief(int x, int y, Door* door) : Entity(x, y), _door(door)
{
	sprites = thiefSprites;
	animTimer = 0.0f;
	doorState = 0;
}

void Thief::Init(Image* clientImage)
{
	//Creación sprites Client
	thiefSprites = new Sprite * [NUM_SPRITES];

	spriteWidth = clientImage->GetWidth() / TOTAL_SPRITES;
	spriteHeight = clientImage->GetHeight();

	for (int i = 0; i < NUM_SPRITES; i++)
	{
		Rect sRect;
		sRect.Width = spriteWidth;
		sRect.Height = spriteHeight;
		sRect.Y = 0;
		sRect.X = (i+1) * sRect.Width;
		thiefSprites[i] = new Sprite(clientImage, sRect);
	}
}

void Thief::Release()
{
	for (int i = 0; i < NUM_SPRITES; ++i)
		delete thiefSprites[i];

	delete[] thiefSprites;
	thiefSprites = nullptr;
}

void Thief::Update(float delta)
{
	int newClientState = 0;
	CheckState(delta, newClientState);

	if (_door->GetCurrentState() != doorState) {
		changedState = true;
		doorState = _door->GetCurrentState();
	}
}

void Thief::Render() {
	if (changedState) {
		Rect partialRect;
		partialRect.X = (currentState+1) * spriteWidth;
		partialRect.Y = 0;
		partialRect.Width = doorState * Door::BACKGROUND_PIXELS;
		partialRect.Height = spriteHeight;

		sprites[currentState]->DrawPartially(_x, _y, partialRect);
	}
}