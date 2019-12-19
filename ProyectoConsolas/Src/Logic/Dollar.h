#pragma once
#include "Entity.h"

class Image;
class Sprite;

/*
	Distintos "sprites" de las "cajas" que indican el estado de una puerta. (Visible o No) (Recibe ingreso o no)
*/
class Dollar : public Entity
{
public:
	static const int NUM_SPRITES;

private:
	enum DollarState { DOLLAR_EMPTY, DOLLAR_VISIBLE, DOLLAR_MONEY, DOLLAR_VISIBLE_MONEY, DOLLAR_ANIM_0, DOLLAR_ANIM_1 };

	static const float ANIM_RATE;

	static Sprite** dollarSprites;

	bool _visible;
	bool _moneyReceived;

	float endAnimTime;
	bool winingMoney;

public:
	static void Init(Image* dollarImage);
	static void Release();

	Dollar(int x, int y);
	void Reset() override;

	void Update(float delta, float time) override;

	inline void SetVisible(bool visible) { _visible = visible; };
	inline void WinMoney() { winingMoney = true; };

	//Si ya esta ganado ese dolar
	inline bool HasAlreadyWonDollar() { return _moneyReceived; };
};

