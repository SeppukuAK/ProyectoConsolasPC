#pragma once

class UserInput
{
public:
	//GAMEPAD
	float HorizontalAxis;	//Joystick izquierdo	[-1.0f,1.0f]
	bool L1, R1;			//Desplazamiento para cambiar puertas
	float L2, R2;			//Gatillos(triggers) analógicos [-1.0f,1.0f]

	//PC
	bool Key_1, Key_2, Key_3, Key_O, Key_P;

	//bool Cross, Square, Triangle, Circle;
	//float VerticalAxis;

	//Constructora
	UserInput() : HorizontalAxis(0.0f), L1(false), R1(false), L2(-1.0f), R2(-1.0f), Key_1(false), Key_2(false), Key_3(false), Key_O(false), Key_P(false) {}

};