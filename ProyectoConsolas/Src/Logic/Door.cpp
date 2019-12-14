#include "Door.h"
#include "../Renderer/Image.h"
#include "../Utilities/Rect.h"
#include "Sprite.h"

const float Door::ANIM_RATE = 1.0f;
const int Door::NUM_SPRITES = 4;

Sprite** Door::doorSprites = nullptr;

Door::Door(int x, int y) : Entity(x, y)
{
	sprites = doorSprites;
	_closed = true;
	animTimer = 0.0f;
}

void Door::Init(Image* doorsImage)
{
	//Creación sprites DoorsAnims
	Rect sRect;

	doorSprites = new Sprite * [NUM_SPRITES];

	sRect.Width = doorsImage->GetWidth() / NUM_SPRITES;
	sRect.Height = doorsImage->GetHeight();
	sRect.Y = 0;

	for (int i = 0; i < NUM_SPRITES; i++)
	{
		sRect.X = i * sRect.Width;
		doorSprites[i] = new Sprite(doorsImage, sRect);
	}
}

void Door::Release()
{
	for (int i = 0; i < NUM_SPRITES; ++i)
		delete[] doorSprites[i];

	delete[] doorSprites;
	doorSprites = nullptr;
}

void Door::Update(float delta, float deltaTime) 
{
	int newDoorState;

	switch (currentState)
	{
		//Se empieza a abrir en este Frame
	case DoorState::DOOR_CLOSED:
		if (_closed)
			newDoorState = DoorState::DOOR_CLOSED;
		else
		{
			animTimer = 0.0f;
			newDoorState = DoorState::DOOR_OPENING_0;
		}
		break;

	case DoorState::DOOR_OPENING_0:
		animTimer += deltaTime;

		if (_closed)
		{
			if (animTimer >= ANIM_RATE)
				newDoorState = DoorState::DOOR_CLOSED;
			else
				newDoorState = DoorState::DOOR_OPENING_0;

		}		
		else
		{
			if (animTimer >= ANIM_RATE / 2)
				newDoorState = DoorState::DOOR_OPENING_1;
			else
				newDoorState = DoorState::DOOR_OPENING_0;
		}
		break;

	case DoorState::DOOR_OPENING_1:
		animTimer += deltaTime;
		if (_closed)
		{
			if (animTimer >= ANIM_RATE / 2)
				newDoorState = DoorState::DOOR_OPENING_0;
			else
				newDoorState = DoorState::DOOR_OPENING_1;

		}
		else
		{
			if (animTimer >= ANIM_RATE)
				newDoorState = DoorState::DOOR_OPENED;
			else
				newDoorState = DoorState::DOOR_OPENING_1;
		}

		break;

	case DoorState::DOOR_OPENED:
		if (_closed)
		{
			animTimer = 0.0f;
			newDoorState = DoorState::DOOR_OPENING_1;
		}
		else
			newDoorState = DoorState::DOOR_OPENED;
		break;
	}

	CheckState(delta, newDoorState);
}
