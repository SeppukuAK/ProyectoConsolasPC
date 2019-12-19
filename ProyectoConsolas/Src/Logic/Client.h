#pragma once

#include "Entity.h"

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
private:
	enum ClientState { CLIENT_IDLE, CLIENT_DEAD_0, CLIENT_DEAD_1 };

	static const float ANIM_RATE;
	static const int NUM_SPRITES;

	static Sprite** clientSprites;

	float animTimer;
	static int spriteWidth;
	static int spriteHeight;

	int doorState;

	Door* _door;

public:
	static void Init(Image* clientImage);
	static void Release();

	Client(int x, int y, Door* door);

	virtual void Update(float delta, float deltaTime) override;
	virtual void Render() override;

};

