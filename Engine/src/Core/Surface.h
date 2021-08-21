#pragma once

#include "Core/Defines.h"

typedef struct Surface Surface;

typedef void Surface_UpdateFunc(Surface* surface);

typedef struct Surface {
    void* _PrivateData;
    Surface_UpdateFunc* Update;
} Surface;

THALLIUM_API b8 Surface_Create(Surface* outSurface, const char* name, u32 width, u32 height);
THALLIUM_API void Surface_Destroy(Surface* surface);

THALLIUM_INLINE void Surface_Update(Surface* surface) {
    if (surface == nil) {
        return;
    }

    surface->Update(surface);
}
