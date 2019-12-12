#pragma once

class Image;
class Sprite;

class Door
{
public:
	static void Init(Image* image);
	static void Release();

	Door();
	~Door();

	void Update(float deltaTime);
	void Render();

private:
	int frameIndex;
	float animTimer;

	static const int DOOR_ANIM_FRAMES;
	static const float ANIM_RATE;
	static Sprite** spritesArray;
};

