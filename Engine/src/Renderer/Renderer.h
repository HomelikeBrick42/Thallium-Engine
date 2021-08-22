#pragma once

#include "Core/Defines.h"
#include "Core/Surface.h"

#include "Containers/String.h"

typedef enum RendererAPI {
    RendererAPI_Vulkan,
    RendererAPI_OpenGL,
} RendererAPI;

typedef struct Renderer Renderer;

typedef void Renderer_DestroyFunc(Renderer* renderer);
typedef void Renderer_OnSurfaceResizeFunc(Renderer* renderer, u32 width, u32 height);
typedef b8 Renderer_BeginFrameFunc(Renderer* renderer, f32 dt);
typedef b8 Renderer_EndFrameFunc(Renderer* renderer, f32 dt);

typedef struct Renderer {
    Renderer_DestroyFunc* _Destroy;
    Renderer_OnSurfaceResizeFunc* _OnSurfaceResize;
    Renderer_BeginFrameFunc* _BeginFrame;
    Renderer_EndFrameFunc* _EndFrame;
    Surface* _Surface;
    void* _PrivateData;
} Renderer;

THALLIUM_API b8 Renderer_Create(Renderer* outRenderer, RendererAPI api, Surface* surface, String name);

THALLIUM_INLINE void Renderer_Destroy(Renderer* renderer) {
    if (renderer == nil || renderer->_Destroy == nil) {
        return;
    }

    renderer->_Destroy(renderer);
}

THALLIUM_INLINE void Renderer_OnSurfaceResize(Renderer* renderer, u32 width, u32 height) {
    if (renderer == nil || renderer->_OnSurfaceResize == nil) {
        return;
    }

    renderer->_OnSurfaceResize(renderer, width, height);
}

THALLIUM_INLINE b8 Renderer_BeginFrame(Renderer* renderer, f32 dt) {
    if (renderer == nil || renderer->_BeginFrame == nil) {
        return FALSE;
    }

    return renderer->_BeginFrame(renderer, dt);
}

THALLIUM_INLINE b8 Renderer_EndFrame(Renderer* renderer, f32 dt) {
    if (renderer == nil || renderer->_EndFrame == nil) {
        return FALSE;
    }

    return renderer->_EndFrame(renderer, dt);
}
