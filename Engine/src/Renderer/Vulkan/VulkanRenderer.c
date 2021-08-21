#include "Renderer/Vulkan/VulkanRenderer.h"

#include "Core/Allocator.h"

b8 VulkanRenderer_Create(Renderer* outRenderer, Surface* surface, String name) {
    (void)name;

    if (outRenderer == nil) {
        return FALSE;
    }

    VulkanRenderer* data = Allocate(sizeof(VulkanRenderer));
    if (data == nil) {
        return FALSE;
    }

    data->Renderer = outRenderer;

    *outRenderer = (Renderer){
        ._Destroy = VulkanRenderer_Destroy,
        ._Surface = surface,
        ._PrivateData = data,
    };

    return TRUE;
}

void VulkanRenderer_Destroy(Renderer* renderer) {
    if (renderer == nil) {
        return;
    }
}
