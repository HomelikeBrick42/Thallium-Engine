#pragma once

#include "Core/Defines.h"
#include "Core/Surface.h"

#include <Windows.h>

typedef struct Win32_Surface {
    Surface* Surface;
    HINSTANCE Instance;
    HWND Handle;
    HDC DeviceContext;
} Win32_Surface;

b8 Win32_Surface_Create(Surface* outSurface, const char* name, u32 width, u32 height);
void Win32_Surface_Destroy(Surface* surface);
void Win32_Surface_Update(Surface* surface);
