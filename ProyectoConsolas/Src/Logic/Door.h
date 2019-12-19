#pragma once

#include "Entity.h"

class Image;
class Sprite;
struct Rect;


class Door : public Entity
{
private:
	/*
		Cuatro posibles posiciones de la puerta. Todos los sprites tienen el mismo ancho.
	*/
	enum DoorState { DOOR_CLOSED, DOOR_OPENING_0, DOOR_OPENING_1, DOOR_OPENED };

	static const float ANIM_RATE;
	static const int NUM_SPRITES;

	static Sprite** doorSprites;
	static Rect* doorRects;

	bool _closed;

	float endAnimTime;

public:
	static const int BACKGROUND_PIXELS;

	static void Init(Image* doorsImage);
	static void Release();

	Door(int x, int y);
	void Reset() override;

	virtual void Update(float delta) override;
	virtual void Render() override;

	inline void SetClosed(bool closed) { _closed = closed; };
	inline int GetCurrentState()const { return currentState; };
	inline bool IsClosed() const { return (currentState == DoorState::DOOR_CLOSED); };
	inline bool IsOpened()const { return (currentState == DoorState::DOOR_OPENED); };
	inline int GetPosX()const { return _x; };
	inline int GetPosY()const { return _y; };

};

