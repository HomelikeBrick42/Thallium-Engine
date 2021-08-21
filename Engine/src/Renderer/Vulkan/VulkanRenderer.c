#define KEEP_FUNCTION_MACROS
#include "Renderer/Vulkan/VulkanRenderer.h"

#include "Core/Allocator.h"

#if defined(THALLIUM_PLATFORM_WINDOWS)
    #include <Windows.h>    
#endif

b8 VulkanRenderer_Create(Renderer* outRenderer, Surface* surface, String name) {
    if (outRenderer == nil) {
        return FALSE;
    }

    VulkanRenderer* data = Allocate(sizeof(VulkanRenderer));
    if (data == nil) {
        return FALSE;
    }

    data->Renderer = outRenderer;

#if defined(THALLIUM_PLATFORM_WINDOWS)
    data->VulkanDllHandle = LoadLibraryA("vulkan-1.dll");
    if (data->VulkanDllHandle == nil) goto Error;

    data->vkGetInstanceProcAddr = cast(PFN_vkGetInstanceProcAddr) GetProcAddress(data->VulkanDllHandle, "vkGetInstanceProcAddr");
    if (data->vkGetInstanceProcAddr == nil) goto Error;
#endif

#define FUNCTION(name) data->name = cast(PFN_ ## name) data->vkGetInstanceProcAddr(nil, #name); if (data->name == nil) goto Error;
    FUNCTIONS
#undef FUNCTION

    const u32 RequiredAPIVersion = VK_API_VERSION_1_2;

    u32 version = 0;
    if (data->vkEnumerateInstanceVersion(&version) != VK_SUCCESS || version < RequiredAPIVersion) goto Error;

    data->Allocator = nil;

    if (data->vkCreateInstance(&(VkInstanceCreateInfo){
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &(VkApplicationInfo){
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = String_ToTempCString(name),
            .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
            .pEngineName = "Thallium",
            .engineVersion = VK_MAKE_VERSION(0, 1, 0),
            .apiVersion = RequiredAPIVersion,
        },
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nil,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = nil,
    }, data->Allocator, &data->Instance) != VK_SUCCESS || data->Instance == VK_NULL_HANDLE) goto Error;

#define INSTANCE_FUNCTION(name) data->name = cast(PFN_ ## name) data->vkGetInstanceProcAddr(data->Instance, #name); if (data->name == nil) goto Error;
    INSTANCE_FUNCTIONS
#undef INSTANCE_FUNCTION

    *outRenderer = (Renderer){
        ._Destroy = VulkanRenderer_Destroy,
        ._Surface = surface,
        ._PrivateData = data,
    };

    return TRUE;
Error:
    Deallocate(data);
    return FALSE;
}

void VulkanRenderer_Destroy(Renderer* renderer) {
    if (renderer == nil) {
        return;
    }
    
    VulkanRenderer* data = renderer->_PrivateData;

    data->vkDestroyInstance(data->Instance, data->Allocator);

#if defined(THALLIUM_PLATFORM_WINDOWS)
    FreeLibrary(data->VulkanDllHandle);
#endif

    Deallocate(data);

    *renderer = (Renderer){};
}
