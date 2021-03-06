#pragma once

#include "Renderer/Renderer.h"

#include <vulkan/vulkan.h>

#if defined(THALLIUM_PLATFORM_WINDOWS)
    #include <Windows.h>
    #include <vulkan/vulkan_win32.h>
    #include "Platform/Win32/WindowsSurface.h"
#endif

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
    VULKAN_INSTANCE_FUNCTION(vkEnumerateDeviceExtensionProperties) \
    VULKAN_INSTANCE_FUNCTION(vkCreateWin32SurfaceKHR) \
    VULKAN_INSTANCE_FUNCTION(vkDestroySurfaceKHR)

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
    VkSurfaceKHR Surface;

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

void VulkanRenderer_OnSurfaceResize(Renderer* renderer, u32 width, u32 height);
b8 VulkanRenderer_BeginFrame(Renderer* renderer, f32 dt);
b8 VulkanRenderer_EndFrame(Renderer* renderer, f32 dt);

#if !defined(KEEP_VULKAN_FUNCTION_MACROS)
    #undef VULKAN_FUNCTIONS
    #undef VULKAN_INSTANCE_FUNCTIONS
#endif
