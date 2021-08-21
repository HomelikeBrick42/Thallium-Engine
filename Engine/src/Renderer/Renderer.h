#pragma once

#include "Core/Defines.h"
#include "Core/Surface.h"

#include "Containers/String.h"

typedef enum RendererAPI {
    RendererAPI_Vulkan,
} RendererAPI;

typedef struct Renderer Renderer;

typedef void Renderer_DestroyFunc(Renderer* renderer);

typedef struct Renderer {
    Renderer_DestroyFunc* _Destroy;
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
