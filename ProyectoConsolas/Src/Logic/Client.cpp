#include "Client.h"
#include "Door.h"
#include "../Renderer/Image.h"
#include "../Utilities/Rect.h"
#include "Sprite.h"
#include <iostream>
#include "../Utilities/Time.h"

using namespace std;

const float Client::ANIM_RATE = 1.0f;
const int Client::NUM_SPRITES = 3;

Sprite** Client::clientSprites = nullptr;
int Client::spriteWidth = 0;
int Client::spriteHeight = 0;

Client::Client(int x, int y, Door* door) : Entity(x, y), _door(door)
{
	sprites = clientSprites;
	animTimer = 0.0f;
	doorState = 0;
}

void Client::Init(Image* clientImage)
{
	//Creación sprites Client
	clientSprites = new Sprite * [NUM_SPRITES];

	spriteWidth = clientImage->GetWidth() / NUM_SPRITES;
	spriteHeight = clientImage->GetHeight();

	for (int i = 0; i < NUM_SPRITES; i++)
	{
		Rect sRect;
		sRect.Width = spriteWidth;
		sRect.Height = spriteHeight;
		sRect.Y = 0;
		sRect.X = i * sRect.Width;
		clientSprites[i] = new Sprite(clientImage, sRect);
	}
}

void Client::Release()
{
	for (int i = 0; i < NUM_SPRITES; ++i)
		delete clientSprites[i];

	delete[] clientSprites;
	clientSprites = nullptr;
}

void Client::Update(float delta)
{
	int newClientState = 0;
	CheckState(Time::time, newClientState);

	if (_door->GetCurrentState() != doorState) {
		changedState = true;
		doorState = _door->GetCurrentState();
	}
}

void Client::Render() {
	if (changedState) {
		Rect partialRect;
		partialRect.X = currentState * spriteWidth;
		partialRect.Y = 0;
		partialRect.Width = doorState * Door::BACKGROUND_PIXELS;
		partialRect.Height = spriteHeight;

		sprites[currentState]->DrawPartially(_x, _y, partialRect);
	}
}