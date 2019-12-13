#include "Sprite.h"
#include "../Renderer/Renderer.h"

Sprite::Sprite(Image* image, Rect sRect) : _image(image), _sRect(sRect)
{
}

void Sprite::Draw(int posX, int posY) {
	Renderer::DrawRect(_image, posX, posY, _sRect);
}

