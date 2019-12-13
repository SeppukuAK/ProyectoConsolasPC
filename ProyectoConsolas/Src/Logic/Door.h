#pragma once

#include "../Renderer/RendererThread.h"

class Image;
class Sprite;

class Door
{
private:
	static const float ANIM_RATE;

	static int _doorAnimFrames;
	static Sprite* doorFrameSprite;
	static Sprite** doorSprites;

	int _x, _y;
	int frameIndex;
	float animTimer;
	RenderCommand drawDoorFrameCommand;
	RenderCommand drawDoorCommand;

public:
	static void Init(Image* doorFrameImage, Image* doorsImage, int doorAnimFrames);
	static void Release();

	Door(int x, int y);
	~Door();

	void Update(float deltaTime);
	void Render();
};

