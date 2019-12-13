#pragma once

/*
	Restringe un valor al rango
*/
static int Clamp(int min, int max, int value)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}

static float Clamp(float min, float max, float value)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}
