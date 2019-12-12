#include "Sprite.h"
#include "../Renderer/Renderer.h"

Sprite::Sprite(Image* image, int sx1, int sy1, int sx2, int sy2) : _image(image), _sx1(sx1), _sy1(sy1), _sx2(sx2), _sy2(sy2)
{

}

void Sprite::Draw(int posX, int posY) {
	Renderer::DrawRect(_image, posX, posY, _sx1, _sy1, _sx2, _sy2);
}

