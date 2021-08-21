#pragma once

#include "Renderer/Renderer.h"

#include <vulkan/vulkan.h>

#define VULKAN_FUNCTIONS \
    VULKAN_FUNCTION(vkCreateInstance) \
    VULKAN_FUNCTION(vkEnumerateInstanceVersion) \
    VULKAN_FUNCTION(vkEnumerateInstanceLayerProperties) \
    VULKAN_FUNCTION(vkEnumerateInstanceExtensionProperties)

#define VULKAN_INSTANCE_FUNCTIONS \
    VULKAN_INSTANCE_FUNCTION(vkDestroyInstance)

typedef struct VulkanRenderer {
    Renderer* Renderer;

    VkAllocationCallbacks* Allocator;
    VkInstance Instance;

    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
    
#define VULKAN_FUNCTION(name) PFN_ ## name name;
    VULKAN_FUNCTIONS
#undef VULKAN_FUNCTION

#define VULKAN_INSTANCE_FUNCTION(name) PFN_ ## name name;
    VULKAN_INSTANCE_FUNCTIONS
#undef VULKAN_INSTANCE_FUNCTION

#if defined(THALLIUM_PLATFORM_WINDOWS)
    void* VulkanDllHandle;
#endif
} VulkanRenderer;

b8 VulkanRenderer_Create(Renderer* outRenderer, Surface* surface, String name);
void VulkanRenderer_Destroy(Renderer* renderer);

#if !defined(KEEP_FUNCTION_MACROS)
    #undef FUNCTIONS
    #undef INSTANCE_FUNCTIONS
#endif
