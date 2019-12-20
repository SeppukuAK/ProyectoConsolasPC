#pragma once
#include "../Entity.h"
#include "../../Utilities/Rect.h"

class Image;
class Sprite;

/*
	Pared con puerta. 3 de ellas crean la escena de juego
*/
class FrameDoor : public Entity
{
private:
	static Sprite* frameDoorSprite;
	static Rect scrollRect;

public:
	static void Init(Image* doorFrameImage);
	static void Release();	
	static int GetFrameDoorWidth() { return scrollRect.Width; };

	FrameDoor(int x, int y);
	void RenderWithDelta(int posX);
};

