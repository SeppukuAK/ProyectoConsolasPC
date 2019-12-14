#pragma once

#include "Entity.h"

class Image;
class Sprite;

class Door : public Entity
{
private:
	enum DoorState { DOOR_CLOSED, DOOR_OPENING_0, DOOR_OPENING_1, DOOR_OPENED };

	static const float ANIM_RATE;
	static const int NUM_SPRITES;

	static Sprite** doorSprites;

	bool _closed;

	float animTimer;

public:
	static void Init(Image* doorsImage);
	static void Release();

	Door(int x, int y);

	void Update(float delta, float deltaTime) override;

	inline void SetClosed(bool closed) { _closed = closed; };

};

