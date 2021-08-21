#pragma once

#include "Renderer/Renderer.h"

#include <vulkan/vulkan.h>

typedef struct VulkanRenderer {
    Renderer* Renderer;
} VulkanRenderer;

b8 VulkanRenderer_Create(Renderer* outRenderer, Surface* surface, String name);
void VulkanRenderer_Destroy(Renderer* renderer);
