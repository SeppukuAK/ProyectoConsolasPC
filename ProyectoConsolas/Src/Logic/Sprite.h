#pragma once

class Image;

class Sprite
{
private:
	Image* _image;
	int _sx1, _sy1, _sx2, _sy2;//Regi�n de la imagen que se va a pintar

public:
	Sprite(Image* image, int sx1, int sy1, int sx2, int sy2);

	//M�todo que pinta el sprite a partir de una posici�n (esquina sup izquierda del sprite)
	void Draw(int posX, int posY);

};

