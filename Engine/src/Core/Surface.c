#include "Core/Surface.h"

#if THALLIUM_PLATFORM_WINDOWS
    #include "Platform/Win32/WindowsSurface.h"
#endif

b8 Surface_Create(Surface* outSurface, const char* name, u32 width, u32 height) {
#if THALLIUM_PLATFORM_WINDOWS
    return Win32_Surface_Create(outSurface, name, width, height);
#endif
}

void Surface_Destroy(Surface* surface) {
#if THALLIUM_PLATFORM_WINDOWS
    Win32_Surface_Destroy(surface);
#endif
}