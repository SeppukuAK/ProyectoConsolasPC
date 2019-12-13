#pragma once

#include "../Renderer/RendererThread.h"

class Image;
class Sprite;

enum DollarState { DOLLAR_EMPTY, DOLLAR_SELECTED, DOLLAR_WIN, DOLLAR_SELECTED_WIN, DOLLAR_ANIM_0, DOLLAR_ANIM_1 };

class Dollar
{

private:
	static const float ANIM_RATE;
	static const int NUM_SPRITES;
	static const int ANIM_FRAMES;

	static Sprite** dollarSprites;
	static Sprite** dollarAnimSprites;

	int _x, _y;
	int frameIndex;
	float animTimer;
	DollarState state;

	RenderCommand drawDollarCommand;
	RenderCommand drawDollarAnimCommand;

public:
	static void Init(Image* dollarImage);
	static void Release();

	Dollar(int x, int y);

	void Update(DollarState newState, float deltaTime);
	void Render();
};

