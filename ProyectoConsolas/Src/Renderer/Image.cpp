#include "Image.h"
#include "Color.h"

Image::Image(Color* colorArray, int width, int height) : _colorArray(colorArray), _width(width), _height(height)
{
}

Image::~Image()
{
	delete[]_colorArray;
	_colorArray = nullptr;
}
