//Fachada alrededor del API de dibujado
//TODO: CREAR LA COMPILACION CONDICIONAL

#if PLATFORM_PS4
#include "Renderer/PS4/RendererPS4.h" //Este tiene todas las cosas extras de la plataforma.

#elif PLATFORM_PC
#include "Renderer/PC/RendererPC.h" //Este tiene todas las cosas extras de la plataforma.

#else
//#error "Plataforma no soportada" TODO: Lanzar un error
#endif

#if PLATFORM_PS4
using Renderer = RendererPS4;

#elif PLATFORM_PC
using Renderer = RendererPC;

#endif