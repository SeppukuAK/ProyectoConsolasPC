//Abstrae los detalles más básicos de la plataforma.

#if PLATFORM_PS4
#include "Platform/PS4/PlatformPS4.h" //Este tiene todas las cosas extras de la plataforma.

#elif PLATFORM_PC
#include "Platform/PC/PlatformPC.h" //Este tiene todas las cosas extras de la plataforma.

#else
//#error "Plataforma no soportada" TODO: Lanzar un error
#endif

#if PLATFORM_PS4
using Platform = PlatformPS4;

#elif PLATFORM_PC
using Platform = PlatformPC;

#endif