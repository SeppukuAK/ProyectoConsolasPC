#pragma once
#include "Entity.h"

class Sprite;
class Image;

/*
	Animaci�n de "Bang" mostrada cuando se recibe un disparo de un ladr�n
*/
class Bang : public Entity
{
private:
	static const int NUM_SPRITES;

	static Sprite** bangSprites;

	float endAnimTime;

public:
	static void Init(Image* bangImage);
	static void Release();

	Bang(int x, int y);
	void Reset() override;

	virtual void Update(float delta) override;
};

