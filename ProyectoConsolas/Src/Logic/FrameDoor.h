#pragma once
#include "Entity.h"

class Image;
class Sprite;


class FrameDoor : public Entity
{
private:

	static Sprite* frameDoorSprite;

public:
	static void Init(Image* doorFrameImage);
	static void Release();

	FrameDoor(int x, int y);

	void Update(float delta, float deltaTime) override;


};

