#pragma once

class UserInput
{
public:
	//PC

	//Teclas
	bool Key_1, Key_2, Key_3, Key_O, Key_P;

	//GAMEPAD

	//Botones
	bool Cross, Square, Triangle, Circle;
	bool L1, R1; //Desplazamiento para cambiar puertas

	//2 triggers analogicos (gatillos) 
	//Va desde -1.0 a 1.0
	//TODO: Hay que convertirlos de analogico a digital
	float L2, R2;

	//joystick izquierdo
	//Va desde -1.0(izq) a 1.0(der) 
	//Zonas muertas: (-0.8 a -0.2) y (0.2 a 0.8)
	float HorizontalAxis;
	float VerticalAxis;

	//Constructora
	UserInput() : Key_1(false), Key_2(false), Key_3(false), Key_O(false), Key_P(false), Cross(false), Square(false), Triangle(false), Circle(false), L1(false), R1(false), L2(-1.0), R2(-1.0), HorizontalAxis(0), VerticalAxis(0) {}

};