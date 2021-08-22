#pragma once

#include "Renderer/Renderer.h"

#include <vulkan/vulkan.h>

#define VULKAN_FUNCTIONS \
    VULKAN_FUNCTION(vkCreateInstance) \
    VULKAN_FUNCTION(vkEnumerateInstanceVersion) \
    VULKAN_FUNCTION(vkEnumerateInstanceLayerProperties) \
    VULKAN_FUNCTION(vkEnumerateInstanceExtensionProperties)

#define VULKAN_INSTANCE_FUNCTIONS \
    VULKAN_INSTANCE_FUNCTION(vkDestroyInstance) \
    VULKAN_INSTANCE_FUNCTION(vkCreateDebugUtilsMessengerEXT) \
    VULKAN_INSTANCE_FUNCTION(vkDestroyDebugUtilsMessengerEXT) \
    VULKAN_INSTANCE_FUNCTION(vkEnumeratePhysicalDevices) \
    VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceProperties) \
    VULKAN_INSTANCE_FUNCTION(vkEnumerateDeviceLayerProperties) \
    VULKAN_INSTANCE_FUNCTION(vkEnumerateDeviceExtensionProperties)

typedef struct VulkanPhysicalDevice {
    VkPhysicalDevice Device;
    VkPhysicalDeviceProperties Properties;
    String DeviceName;
} VulkanPhysicalDevice;

typedef struct VulkanRenderer {
    Renderer* Renderer;

    VkAllocationCallbacks* Allocator;
    VkInstance Instance;
#if !defined(THALLIUM_RELEASE)
    VkDebugUtilsMessengerEXT DebugMessenger;
#endif
    VulkanPhysicalDevice PhysicalDevice;

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

#if !defined(KEEP_VULKAN_FUNCTION_MACROS)
    #undef VULKAN_FUNCTIONS
    #undef VULKAN_INSTANCE_FUNCTIONS
#endif
