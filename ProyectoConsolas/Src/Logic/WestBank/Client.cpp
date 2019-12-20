#include "Client.h"
#include "Door.h"
#include "../../Renderer/Image.h"
#include "../../Utilities/Rect.h"
#include "../Sprite.h"
#include <iostream>
#include "../../Utilities/Time.h"

using namespace std;

const float Client::ANIM_RATE = 0.5f;
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

	_dying = false;
	_dead = false;
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

void Client::Update(float delta)
{
	int newClientState = 0;

	if (_dying) {

		switch (currentState) {
		case ClientState::CLIENT_DEAD_0:
			//Fin de la animaci�n
			if (Time::time >= endAnimTime - ANIM_RATE / 2) {
				_dead = true;
				newClientState = ClientState::CLIENT_DEAD_1;
			}
			else 
				newClientState = ClientState::CLIENT_DEAD_0;

			break;
		case ClientState::CLIENT_DEAD_1:
				newClientState = ClientState::CLIENT_DEAD_1;

			break;
		case ClientState::CLIENT_IDLE:
			//Se inicia la animaci�n
			endAnimTime = Time::time + ANIM_RATE;
			newClientState = ClientState::CLIENT_DEAD_0;
			break;
		}
	}
	else if(!_dying && !_dead) {
		newClientState = ClientState::CLIENT_IDLE;
	}
	CheckState(delta, newClientState);

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