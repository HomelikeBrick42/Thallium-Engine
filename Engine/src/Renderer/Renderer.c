#include "Renderer/Renderer.h"

#include "Renderer/Vulkan/VulkanRenderer.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"

b8 Renderer_Create(Renderer* outRenderer, RendererAPI api, Surface* surface, String name) {
    if (outRenderer == nil || surface == nil) {
        return FALSE;
    }

    switch (api) {
        case RendererAPI_Vulkan: {
            return VulkanRenderer_Create(outRenderer, surface, name);
        } break;

        case RendererAPI_OpenGL: {
            return OpenGLRenderer_Create(outRenderer, surface, name);
        } break;

        default: {
            return FALSE;
        } break;
    }
}
