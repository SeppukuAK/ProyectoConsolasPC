#pragma once

#include "../Utilities/Rect.h"

class Image;

class Sprite
{
private:
	Image* _image;
	Rect _sRect;		//Región de la imagen que se va a pintar

public:
	Sprite(Image* image, Rect sRect);

	//Método que pinta el sprite a partir de una posición (esquina sup izquierda del sprite)
	void Draw(int posX, int posY);

};

