//Se envuelve todo para evitar que se compile si no estamos en la plataforma
#if PLATFORM_PS4

#include "RendererPS4.h"

//TODO: Implementar los métodos
void RendererPS4::Init() {}
void RendererPS4::Release() {}

void RendererPS4::Clear(Color color) {}
void RendererPS4::PutPixel(int x, int y, Color color) {}
void RendererPS4::Present() {}

#endif
