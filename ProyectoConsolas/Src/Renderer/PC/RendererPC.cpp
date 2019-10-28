//Se envuelve todo para evitar que se compile si no estamos en la plataforma
#if PLATFORM_PC
#include "RendererPC.h"

//TODO: Implementar los métodos
static void RendererPC::Clear() {}
static void RendererPC::PutPixel() {}
static void RendererPC::Present() {}
#endif