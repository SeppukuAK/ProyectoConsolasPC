//Se envuelve todo para evitar que se compile si no estamos en la plataforma
#if PLATFORM_PC
#include "RendererPC.h"

//TODO: Implementar los métodos
void RendererPC::Init() {}
void RendererPC::Release() {}

void RendererPC::Clear(Color color) {}
void RendererPC::PutPixel(int x, int y, Color color) {}
void RendererPC::Present() {}

int RendererPC::GetWidth() { return 0; }
int RendererPC::GetHeight(){ return 0; }
#endif