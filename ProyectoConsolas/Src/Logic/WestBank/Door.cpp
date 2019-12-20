#include "Door.h"
#include "../../Renderer/Image.h"
#include "../../Utilities/Rect.h"
#include "../Sprite.h"
#include <iostream>
#include "../../Utilities/Time.h"

using namespace std;

const float Door::ANIM_RATE = 0.5f;
const int Door::NUM_SPRITES = 4;
const int Door::BACKGROUND_PIXELS = 32;

Sprite** Door::doorSprites = nullptr;
Rect* Door::doorRects = nullptr;

Door::Door(int x, int y) : Entity(x, y)
{
	sprites = doorSprites;
	_closed = true;
	endAnimTime = 0.0f;
}

void Door::Reset()
{
	Entity::Reset();

	endAnimTime = 0.0f;
	_closed = true;
}


void Door::Init(Image* doorsImage)
{
	//Creaci�n sprites DoorsAnims
	doorSprites = new Sprite * [NUM_SPRITES];
	doorRects = new Rect[NUM_SPRITES];

	for (int i = 0; i < NUM_SPRITES; i++)
	{
		Rect sRect;
		sRect.Width = doorsImage->GetWidth() / NUM_SPRITES;
		sRect.Height = doorsImage->GetHeight();
		sRect.Y = 0;
		sRect.X = i * sRect.Width;
		doorSprites[i] = new Sprite(doorsImage, sRect);

		sRect.Width -= BACKGROUND_PIXELS * i;
		sRect.X += BACKGROUND_PIXELS * i;
		doorRects[i] = sRect;
	}
}

void Door::Release()
{
	for (int i = 0; i < NUM_SPRITES; ++i)
		delete doorSprites[i];

	delete[] doorSprites;
	doorSprites = nullptr;

	delete doorRects;
	doorRects = nullptr;
}

void Door::Update(float delta)
{
	int newDoorState = 0;

	switch (currentState)
	{
		//Se empieza a abrir en este Frame
	case DoorState::DOOR_CLOSED:
		if (_closed)
			newDoorState = DoorState::DOOR_CLOSED;

		//Se empieza a abrir
		else
		{
			endAnimTime = Time::time + ANIM_RATE;
			newDoorState = DoorState::DOOR_OPENING_0;
		}
		break;

	case DoorState::DOOR_OPENING_0:
		if (_closed)
		{
			if (Time::time >= endAnimTime)
				newDoorState = DoorState::DOOR_CLOSED;
			else
				newDoorState = DoorState::DOOR_OPENING_0;

		}
		else
		{
			if (Time::time >= endAnimTime - ANIM_RATE / 2)
				newDoorState = DoorState::DOOR_OPENING_1;
			else
				newDoorState = DoorState::DOOR_OPENING_0;
		}
		break;

	case DoorState::DOOR_OPENING_1:
		if (_closed)
		{
			if (Time::time >= endAnimTime - ANIM_RATE / 2)
				newDoorState = DoorState::DOOR_OPENING_0;
			else
				newDoorState = DoorState::DOOR_OPENING_1;

		}
		else
		{
			if (Time::time >= endAnimTime)
				newDoorState = DoorState::DOOR_OPENED;
			else
				newDoorState = DoorState::DOOR_OPENING_1;
		}

		break;

	case DoorState::DOOR_OPENED:
		if (_closed)
		{
			endAnimTime = Time::time + ANIM_RATE;
			newDoorState = DoorState::DOOR_OPENING_1;
		}
		else
			newDoorState = DoorState::DOOR_OPENED;
		break;
	}

	CheckState(delta, newDoorState);
}

void Door::Render() {
	if (changedState)
		sprites[currentState]->DrawPartially(_x + BACKGROUND_PIXELS * currentState, _y, doorRects[currentState]);
}

