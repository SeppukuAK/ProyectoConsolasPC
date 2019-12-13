#pragma once

/*
	Fachada alrededor del API de dibujado
*/

//Se incluye el correspondiente a la plataforma
#if PLATFORM_PS4

#include "PS4/RendererPS4.h" 
using Renderer = RendererPS4;

#elif PLATFORM_PC

#include "PC/RendererPC.h"
using Renderer = RendererPC;

#else

//#error "Plataforma no soportada" TODO: Lanzar un error

#endif