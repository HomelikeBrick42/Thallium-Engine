#pragma once

#include "Core/Defines.h"

typedef struct Surface Surface;

typedef void Surface_DestroyFunc(Surface* surface);
typedef void Surface_UpdateFunc(Surface* surface);
typedef void Surface_OnCloseCallbackFunc(Surface* surface);

typedef struct Surface {
    Surface_OnCloseCallbackFunc* OnCloseCallback;
    Surface_DestroyFunc* _Destroy;
    Surface_UpdateFunc* _Update;
    void* _PrivateData;
} Surface;

THALLIUM_API b8 Surface_Create(Surface* outSurface, const char* name, u32 width, u32 height);

THALLIUM_INLINE void Surface_Destroy(Surface* surface) {
    if (surface == nil) {
        return;
    }

    surface->_Destroy(surface);
}

THALLIUM_INLINE void Surface_Update(Surface* surface) {
    if (surface == nil) {
        return;
    }

    surface->_Update(surface);
}
