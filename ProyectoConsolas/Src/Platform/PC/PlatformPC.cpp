//Se envuelve todo para evitar que se compile si no estamos en la plataforma
#if PLATFORM_PC

#include "PlatformPC.h"

//TODO: Implementar los métodos
void PlatformPC::Init() {}
void PlatformPC::Release() {}
bool PlatformPC::Tick() { return true; }

#endif
