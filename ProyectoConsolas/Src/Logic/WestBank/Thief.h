#pragma once

#include "../Entity.h"

class Image;
class Sprite;
struct Rect;
class Door;

/*
	Tienen el mismo ancho que el ancho que deja una puerta completamente abierta
	TODO: LADRON TAMBIEN
*/
class Thief : public Entity
{
public:
	static const int NUM_SPRITES;

private:
	enum ThiefState { THIEF_IDLE, THIEF_DEAD_0, THIEF_DEAD_1 };

	static const float ANIM_RATE;
	static const int TOTAL_SPRITES;

	static Sprite** thiefSprites;

	float endAnimTime;

	bool _dying;
	bool _dead;

	float animTimer;
	static int spriteWidth;
	static int spriteHeight;

	int doorState;

	Door* _door;

public:
	static void Init(Image* thiefImage);
	static void Release();

	Thief(int x, int y, Door* door);

	virtual void Update(float delta) override;
	virtual void Render() override;

	inline void Die() { _dying = true; };
	inline bool IsDead() { return (_dead == true); };

};

