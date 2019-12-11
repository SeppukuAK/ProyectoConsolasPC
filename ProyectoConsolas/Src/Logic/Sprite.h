#pragma once

class Image;

class Sprite
{
private:
	Image* image;
	int posX, posY;
	int width, height;
	int offsetX, offsetY;

public:
	inline Image* GetImage()const { return image; };
	inline int GetPosX()const { return posX; };
	inline int GetPosY()const { return posY; };
	inline int GetWidth()const { return width; };
	inline int GetHeight()const { return height; };
	inline int GetOffsetX()const { return offsetX; };
	inline int GetOffsetY()const { return offsetY; };

};

