#pragma once
#include "Entity.h"
#include "../Utilities/Rect.h"

class Image;
class Sprite;

/*
	Pared con puerta
*/
class FrameDoor : public Entity
{
private:
	static Sprite* frameDoorSprite;
	static Rect scrollRect;

public:
	static void Init(Image* doorFrameImage);
	static void Release();

	FrameDoor(int x, int y);

	void Update(float delta, float time) override;
	//virtual void Render() override;
	void RenderWithDelta(int posX);
	static int GetFrameDoorWidth() { return scrollRect.Width; };
};

