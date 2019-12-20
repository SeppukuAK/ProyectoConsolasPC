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
class Client : public Entity
{
public: 
	static const int NUM_SPRITES;

private:
	enum ClientState { CLIENT_IDLE, CLIENT_DEAD_0, CLIENT_DEAD_1 };

	static const float ANIM_RATE;

	static Sprite** clientSprites;

	float endAnimTime;

	bool _dying;
	bool _dead;

	float animTimer;
	static int spriteWidth;
	static int spriteHeight;

	int doorState;

	Door* _door;

public:
	static void Init(Image* clientImage);
	static void Release();

	Client(int x, int y, Door* door);

	virtual void Update(float delta) override;
	virtual void Render() override;

	inline void Die() { _dying = true; };
	inline bool IsDead() { return (_dead == true); };

};

