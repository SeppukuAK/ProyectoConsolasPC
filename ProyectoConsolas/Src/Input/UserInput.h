#pragma once
class UserInput
{
public:
	//Botones
	bool Circle;
	bool Cross;
	bool Triangle;
	bool Square;
	bool R1, L1; //Desplazamiento para cambiar puertas

	//2 triggers analogicos (gatillos) 
	//Va desde -1.0 a 1.0
	//TODO: Hay que convertirlos de analogico a digital
	float L2, R2;

	//joystick izquierdo
	//Va desde -1.0(izq) a 1.0(der) 
	//Zonas muertas: (-0.8 a -0.2) y (0.2 a 0.8)
	float LeftJoystickHor;
	float LeftJoystickVer;


	//Constructora
	UserInput() : Circle(false), Cross(false), Triangle(false), Square(false), L2(-1.0), R2(-1.0), LeftJoystickHor(0), LeftJoystickVer(0) {}

};