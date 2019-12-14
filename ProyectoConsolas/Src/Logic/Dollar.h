#pragma once

class Image;
class Sprite;

enum DollarState { DOLLAR_EMPTY, DOLLAR_VISIBLE, DOLLAR_MONEY, DOLLAR_VISIBLE_MONEY, DOLLAR_ANIM_0, DOLLAR_ANIM_1 };

class Dollar
{
private:
	static const float ANIM_RATE;
	static const int NUM_SPRITES;

	static Sprite** dollarSprites;

	int _x, _y;
	bool _visible;
	bool _moneyReceived;
	bool changedState;
	DollarState dollarState;
	int lastFrameStateChanged;		//Ultimo tick en el que ha habido un cambio de estado
	float animTimer;

public:
	static void Init(Image* dollarImage);
	static void Release();

	Dollar(int x, int y);

	void Update(float delta, float deltaTime);
	void Render();

	inline bool GetMoneyReceived() const { return _moneyReceived; };
	inline void SetVisible(bool visible) { _visible = visible; };
	inline void SetMoneyReceived(bool moneyReceived) { _moneyReceived = moneyReceived; };
};

