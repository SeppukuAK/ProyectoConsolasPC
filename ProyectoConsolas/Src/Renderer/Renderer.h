#pragma once

/*
	Fachada alrededor del API de dibujado
*/

//Se incluye el correspondiente a la plataforma
#if PLATFORM_PS4
#include "PS4/RendererPS4.h" 

#elif PLATFORM_PC
#include "PC/RendererPC.h"

#else
//#error "Plataforma no soportada" TODO: Lanzar un error
#endif

#if PLATFORM_PS4
using Renderer = RendererPS4;

#elif PLATFORM_PC
using Renderer = RendererPC;

#endif