#pragma once
#include "Entity.h"

class Image;
class Sprite;


class Dollar : public Entity
{
private:
	enum DollarState { DOLLAR_EMPTY, DOLLAR_VISIBLE, DOLLAR_MONEY, DOLLAR_VISIBLE_MONEY, DOLLAR_ANIM_0, DOLLAR_ANIM_1 };

	static const float ANIM_RATE;
	static const int NUM_SPRITES;

	static Sprite** dollarSprites;

	bool _visible;
	bool _moneyReceived;
	float animTimer;

public:
	static void Init(Image* dollarImage);
	static void Release();

	Dollar(int x, int y);

	void Update(float delta, float deltaTime) override;

	inline bool GetMoneyReceived() const { return _moneyReceived; };
	inline void SetVisible(bool visible) { _visible = visible; };
	inline void SetMoneyReceived(bool moneyReceived) { _moneyReceived = moneyReceived; };
};

