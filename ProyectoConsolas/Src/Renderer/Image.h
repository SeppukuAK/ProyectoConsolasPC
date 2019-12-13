#pragma once

class Color;

class Image
{
private:
	Color* _colorArray;
	int _width, _height;

public:
	Image(Color* colorArray, int width, int height);
	~Image();

	inline Color* GetColorArray() const { return _colorArray; }
	inline int GetWidth() const { return _width; }
	inline int GetHeight() const { return _height; }
};

