#pragma once

class Image;
class Sprite;

enum DollarState { DOLLAR_EMPTY, DOLLAR_SELECTED, DOLLAR_WIN, DOLLAR_SELECTED_WIN, DOLLAR_ANIM_0, DOLLAR_ANIM_1 };

class Dollar
{

private:

	static const int NUM_SPRITES;

	static Sprite** dollarSprites;

	int _x, _y;
	DollarState state;

public:
	static void Init(Image* dollarImage);
	static void Release();

	Dollar(int x, int y);

	void Update(DollarState newState);
	void Render();
};

