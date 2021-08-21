#pragma once

#include "Renderer/Renderer.h"

#include <vulkan/vulkan.h>

#define FUNCTIONS \
    FUNCTION(vkCreateInstance) \
    FUNCTION(vkEnumerateInstanceVersion) \
    FUNCTION(vkEnumerateInstanceLayerProperties) \
    FUNCTION(vkEnumerateInstanceExtensionProperties)

#define INSTANCE_FUNCTIONS \
    INSTANCE_FUNCTION(vkDestroyInstance)

typedef struct VulkanRenderer {
    Renderer* Renderer;

    VkAllocationCallbacks* Allocator;
    VkInstance Instance;

    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
    
#define FUNCTION(name) PFN_ ## name name;
    FUNCTIONS
#undef FUNCTION

#define INSTANCE_FUNCTION(name) PFN_ ## name name;
    INSTANCE_FUNCTIONS
#undef INSTANCE_FUNCTION

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
