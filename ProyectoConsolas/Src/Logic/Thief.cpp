#include "Thief.h"
#include "Door.h"
#include "../Renderer/Image.h"
#include "../Utilities/Rect.h"
#include "Sprite.h"
#include <iostream>
#include "../Utilities/Time.h"

using namespace std;

const float Thief::ANIM_RATE = 0.5f;
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
	endAnimTime = 0.0f;

	_dying = false;
	_dead = false;
}

void Thief::Init(Image* thiefImage)
{
	spriteWidth = thiefImage->GetWidth() / TOTAL_SPRITES;
	spriteHeight = thiefImage->GetHeight();

	Rect sRect;
	sRect.Width = spriteWidth;
	sRect.Height = spriteHeight;
	sRect.Y = 0;
	sRect.X = 0;

	//Creaci�n sprites Thief
	thiefSprites = new Sprite * [NUM_SPRITES];

	for (int i = 0; i < NUM_SPRITES; i++)
	{		
		sRect.X = (i+1) * sRect.Width;
		thiefSprites[i] = new Sprite(thiefImage, sRect);
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
	int newThiefState = 0;

	if (_dying) {

		switch (currentState) {
		case ThiefState::THIEF_DEAD_0:
			//Fin de la animaci�n
			if (Time::time >= endAnimTime - ANIM_RATE / 2) {
				_dead = true;
				//_dying = false;//TODO: LO ARREGLAR� ESTO?
				newThiefState = ThiefState::THIEF_DEAD_1;
			}
			else
				newThiefState = ThiefState::THIEF_DEAD_0;

			break;

		case ThiefState::THIEF_DEAD_1:
			newThiefState = ThiefState::THIEF_DEAD_1;

			break;
		case ThiefState::THIEF_IDLE:
			//Se inicia la animaci�n
			endAnimTime = Time::time + ANIM_RATE;
			newThiefState = ThiefState::THIEF_DEAD_0;
			break;
		}
	}
	else if (!_dying && !_dead) {
		newThiefState = ThiefState::THIEF_IDLE;
	}

	CheckState(delta, newThiefState);

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