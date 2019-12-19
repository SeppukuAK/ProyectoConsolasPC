#include "Client.h"
#include "Door.h"
#include "../Renderer/Image.h"
#include "../Utilities/Rect.h"
#include "Sprite.h"
#include <iostream>

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
	endAnimTime = 0.0f;
	dying = false;
}

void Client::Reset()
{
	Entity::Reset();

	endAnimTime = 0.0f;
	dying = false;
}

void Client::Init(Image* clientImage)
{
	spriteWidth = clientImage->GetWidth() / NUM_SPRITES;
	spriteHeight = clientImage->GetHeight();

	Rect sRect;
	sRect.Width = spriteWidth;
	sRect.Height = spriteHeight;
	sRect.Y = 0;
	sRect.X = 0;

	//Creaci�n sprites Client
	clientSprites = new Sprite * [NUM_SPRITES];

	for (int i = 0; i < NUM_SPRITES; i++)
	{
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

void Client::Update(float delta, float time)
{
	int newClientState = 0;

	if (dying) {
		switch (currentState) {
		case ClientState::CLIENT_DEAD_0:
			if (time >= endAnimTime - ANIM_RATE / 2) {
				newClientState = ClientState::CLIENT_DEAD_1;
				dying = false;
			}

		case ClientState::CLIENT_IDLE:
			//Se inicia la animaci�n
			endAnimTime = time + ANIM_RATE;
			newClientState = ClientState::CLIENT_DEAD_0;
			break;
		}
		CheckState(delta, newClientState);
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