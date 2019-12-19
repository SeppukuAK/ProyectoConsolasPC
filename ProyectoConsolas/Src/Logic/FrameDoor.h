#pragma once
#include "Entity.h"

class Image;
class Sprite;

/*
	Pared con puerta
*/
class FrameDoor : public Entity
{
private:
	static Sprite* frameDoorSprite;

public:
	static void Init(Image* doorFrameImage);
	static void Release();

	FrameDoor(int x, int y);

	void Update(float delta, float time) override;
};

