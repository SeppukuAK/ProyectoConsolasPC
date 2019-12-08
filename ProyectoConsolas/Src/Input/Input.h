#pragma once

/*
	Fachada alrededor del API de Input
*/

//Se incluye el correspondiente a la plataforma
#if PLATFORM_PS4
#include "PS4/InputPS4.h" 

#elif PLATFORM_PC
#include "PC/InputPC.h"

#else
//#error "Plataforma no soportada" TODO: Lanzar un error
#endif

#if PLATFORM_PS4
using Input = InputPS4;

#elif PLATFORM_PC
using Input = InputPC;

#endif