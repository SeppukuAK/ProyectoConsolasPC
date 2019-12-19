#pragma once

#include "../Utilities/Rect.h"

class Image;

class Sprite
{
private:
	Image* _image;
	Rect _sRect;		//Regi�n de la imagen que se va a pintar

public:
	Sprite(Image* image, Rect sRect);

	//M�todo que pinta el sprite a partir de una posici�n (esquina sup izquierda del sprite)
	void Draw(int posX, int posY);
	void DrawPartially(int posX, int posY, Rect partialRect);
};

