#pragma once

/*
	Fachada alrededor de la inicialización de la plataforma
*/

//Se incluye el correspondiente a la plataforma
#if PLATFORM_PS4
#include "PS4/PlatformPS4.h" 

#elif PLATFORM_PC
#include "PC/PlatformPC.h" 

#else
//#error "Plataforma no soportada" TODO: Lanzar un error
#endif

#if PLATFORM_PS4
using Platform = PlatformPS4;

#elif PLATFORM_PC
using Platform = PlatformPC;

#endif