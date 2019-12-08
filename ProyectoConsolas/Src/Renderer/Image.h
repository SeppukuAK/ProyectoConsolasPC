#pragma once

#include "Utilities/Color.h"

class Image
{
private:
	Color* _colorArray;
	int _width, _height;

public:
	Image(Color* colorArray, int width, int height);
	~Image();

	Color* GetColorArray() { return _colorArray; }
	int GetWidth() { return _width; }
	int GetHeight() { return _height; }
};

