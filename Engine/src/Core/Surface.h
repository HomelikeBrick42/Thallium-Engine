#pragma once

#include "Core/Defines.h"
#include "Core/KeyCode.h"

#include "Containers/String.h"

typedef struct Surface Surface;

typedef void Surface_DestroyFunc(Surface* surface);
typedef void Surface_UpdateFunc(Surface* surface);

typedef void Surface_OnCloseCallbackFunc(Surface* surface);
typedef void Surface_OnKeyCallbackFunc(Surface* surface, KeyCode key, b8 pressed);

typedef struct Surface {
    void* UserData;
    Surface_OnCloseCallbackFunc* OnCloseCallback;
    Surface_OnKeyCallbackFunc* OnKeyCallback;
    Surface_DestroyFunc* _Destroy;
    Surface_UpdateFunc* _Update;
    void* _PrivateData;
} Surface;

THALLIUM_API b8 Surface_Create(Surface* outSurface, String name, u32 width, u32 height);

THALLIUM_INLINE void Surface_Destroy(Surface* surface) {
    if (surface == nil || surface->_Destroy == nil) {
        return;
    }

    surface->_Destroy(surface);
}

THALLIUM_INLINE void Surface_Update(Surface* surface) {
    if (surface == nil || surface->_Update == nil) {
        return;
    }

    surface->_Update(surface);
}
