#define KEEP_VULKAN_FUNCTION_MACROS
#include "Renderer/Vulkan/VulkanRenderer.h"

#include "Core/Allocator.h"
#include "Core/Logger.h"

#if defined(THALLIUM_PLATFORM_WINDOWS)
    #include <Windows.h>    
#endif

#if !defined(THALLIUM_RELEASE)
VkBool32 VKAPI_CALL DebugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
);
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

    *outRenderer = (Renderer){
        ._Destroy = VulkanRenderer_Destroy,
        ._Surface = surface,
        ._PrivateData = data,
    };

#if defined(THALLIUM_PLATFORM_WINDOWS)
    data->VulkanDllHandle = LoadLibraryA("vulkan-1.dll");
    if (data->VulkanDllHandle == nil) goto Error;

    data->vkGetInstanceProcAddr = cast(PFN_vkGetInstanceProcAddr) GetProcAddress(data->VulkanDllHandle, "vkGetInstanceProcAddr");
    if (data->vkGetInstanceProcAddr == nil) goto Error;
#endif

    LogDebug(String_FromLiteral("Getting Vulkan Functions"));
#define VULKAN_FUNCTION(name) data->name = cast(PFN_ ## name) data->vkGetInstanceProcAddr(nil, #name); if (data->name == nil) goto Error;
    VULKAN_FUNCTIONS
#undef VULKAN_FUNCTION
    goto Error;

    const u32 RequiredAPIVersion = VK_API_VERSION_1_2;

    {
        u32 version = 0;
        if (data->vkEnumerateInstanceVersion(&version) != VK_SUCCESS || version < RequiredAPIVersion) goto Error;

        const char* InstanceLayers[] = {
#if !defined(THALLIUM_RELEASE)
            "VK_LAYER_KHRONOS_validation",
#endif
        };

        const char* InstanceExtensions[] = {
#if !defined(THALLIUM_RELEASE)
	        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
        };

        data->Allocator = nil;

        data->Instance = VK_NULL_HANDLE;
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
            .enabledLayerCount = sizeof(InstanceLayers) / sizeof(InstanceLayers[0]),
            .ppEnabledLayerNames = InstanceLayers,
            .enabledExtensionCount = sizeof(InstanceExtensions) / sizeof(InstanceExtensions[0]),
            .ppEnabledExtensionNames = InstanceExtensions,
        }, data->Allocator, &data->Instance) != VK_SUCCESS || data->Instance == VK_NULL_HANDLE) goto Error;
        LogDebug(String_FromLiteral("Created Vulkan Instance"));
    }

    LogDebug(String_FromLiteral("Getting Vulkan Instance Functions"));
#define VULKAN_INSTANCE_FUNCTION(name) data->name = cast(PFN_ ## name) data->vkGetInstanceProcAddr(data->Instance, #name); if (data->name == nil) goto Error;
    VULKAN_INSTANCE_FUNCTIONS
#undef VULKAN_INSTANCE_FUNCTION

#if !defined(THALLIUM_RELEASE)
    data->DebugMessenger = VK_NULL_HANDLE;
    if (data->vkCreateDebugUtilsMessengerEXT(data->Instance, &(VkDebugUtilsMessengerCreateInfoEXT){
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity =
              VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType =
              VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = DebugMessengerCallback,
        .pUserData = data,
    }, data->Allocator, &data->DebugMessenger) != VK_SUCCESS || data->DebugMessenger == VK_NULL_HANDLE) goto Error;
    LogDebug(String_FromLiteral("Created Vulkan Debug Messenger"));
#endif

    LogDebug(String_FromLiteral("Created Vulkan Renderer"));
    return TRUE;
Error:
    LogError(String_FromLiteral("Failed to create Vulkan Renderer!"));
    VulkanRenderer_Destroy(outRenderer);
    return FALSE;
}

void VulkanRenderer_Destroy(Renderer* renderer) {
    if (renderer == nil) {
        return;
    }
    
    VulkanRenderer* data = renderer->_PrivateData;

    if (data->Instance != VK_NULL_HANDLE) {
#if !defined(THALLIUM_RELEASE)
        if (data->DebugMessenger != VK_NULL_HANDLE) {
            data->vkDestroyDebugUtilsMessengerEXT(data->Instance, data->DebugMessenger, data->Allocator);
            LogDebug(String_FromLiteral("Destroyed Vulkan Debug Messenger"));
        }
#endif
        data->vkDestroyInstance(data->Instance, data->Allocator);
        LogDebug(String_FromLiteral("Destroyed Vulkan Instance"));
    }

#if defined(THALLIUM_PLATFORM_WINDOWS)
    FreeLibrary(data->VulkanDllHandle);
#endif

    Deallocate(data);

    *renderer = (Renderer){};

    LogDebug(String_FromLiteral("Destroyed Vulkan Renderer"));
}

#if !defined(THALLIUM_RELEASE)
// TODO:
static VkBool32 VKAPI_CALL DebugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
) {
    (void)messageTypes;
    (void)pUserData;

    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
            LogTrace(String_FromLiteral("Vulkan message: %s"), String_FromCString(pCallbackData->pMessage));
        } break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
            LogDebug(String_FromLiteral("Vulkan message: %s"), String_FromCString(pCallbackData->pMessage));
        } break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            LogWarn(String_FromLiteral("Vulkan message: %s"), String_FromCString(pCallbackData->pMessage));
        } break;

        default:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            LogError(String_FromLiteral("Vulkan message: %s"), String_FromCString(pCallbackData->pMessage));
        } break;
    }

    return VK_FALSE;
}
#endif
