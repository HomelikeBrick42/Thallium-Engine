#define KEEP_VULKAN_FUNCTION_MACROS
#include "Renderer/Vulkan/VulkanRenderer.h"

#include "Core/Allocator.h"
#include "Core/Logger.h"

#if defined(THALLIUM_PLATFORM_WINDOWS)
    #include <Windows.h>    
#endif

static const char* InstanceLayers[] = {
#if !defined(THALLIUM_RELEASE)
    "VK_LAYER_KHRONOS_validation",
#endif
};

static const char* InstanceExtensions[] = {
#if !defined(THALLIUM_RELEASE)
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
#if defined(THALLIUM_PLATFORM_WINDOWS)
    "VK_KHR_win32_surface",
#endif
    VK_KHR_SURFACE_EXTENSION_NAME,
};

static const char* DeviceLayers[] = {
};

static const char* DeviceExtensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

#if !defined(THALLIUM_RELEASE)
VkBool32 VKAPI_CALL DebugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
);
#endif

b8 VulkanRenderer_Create(Renderer* outRenderer, Surface* surface, String name) {
#define VK_CHECK(x) if ((x) != VK_SUCCESS) goto Error

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

#define VULKAN_FUNCTION(name) data->name = cast(PFN_ ## name) data->vkGetInstanceProcAddr(nil, #name); if (data->name == nil) { LogDebug(String_FromLiteral("Failed to get Vulkan Function '" #name "'")); goto Error; }
    VULKAN_FUNCTIONS
#undef VULKAN_FUNCTION
    LogDebug(String_FromLiteral("Got Vulkan Functions"));

    const u32 RequiredAPIVersion = VK_API_VERSION_1_2;

    data->Allocator = nil;

    {
        u32 version = 0;
        if (data->vkEnumerateInstanceVersion(&version) != VK_SUCCESS || version < RequiredAPIVersion) goto Error;

        {
            u32 availableLayerCount = 0;
            VK_CHECK(data->vkEnumerateInstanceLayerProperties(&availableLayerCount, nil));
            VkLayerProperties availableLayers[availableLayerCount];
            VK_CHECK(data->vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers));

            b8 hasLayers = TRUE;
            for (u64 i = 0; i < (sizeof(InstanceLayers) / sizeof(InstanceLayers[0])); i++) {
                b8 found = FALSE;
                String layerString = String_FromCString(InstanceLayers[i]);

                for (u64 j = 0; j < availableLayerCount; j++) {
                    String availableLayerString = String_FromCString(availableLayers[j].layerName);
                    if (String_Equal(layerString, availableLayerString)) {
                        found = TRUE;
                        break;
                    }
                }

                if (!found) {
                    LogError(String_FromLiteral("Failed to find vulkan instance layer '%s'"), layerString);
                    hasLayers = FALSE;
                }
            }
            if (!hasLayers) goto Error;
        }
        LogDebug(String_FromLiteral("Found all required Vulkan Instance Layers"));

        {
            u32 availableExtensionCount = 0;
            VK_CHECK(data->vkEnumerateInstanceExtensionProperties(nil, &availableExtensionCount, nil));
            VkExtensionProperties availableExtensions[availableExtensionCount];
            VK_CHECK(data->vkEnumerateInstanceExtensionProperties(nil, &availableExtensionCount, availableExtensions));

            b8 hasExtensions = TRUE;
            for (u64 i = 0; i< (sizeof(InstanceExtensions) / sizeof(InstanceExtensions[0])); i++) {
                b8 found = FALSE;
                String extensionString = String_FromCString(InstanceExtensions[i]);

                for (u64 j = 0; j < availableExtensionCount; j++) {
                    String availableExtensionString = String_FromCString(availableExtensions[j].extensionName);
                    if (String_Equal(extensionString, availableExtensionString)) {
                        found = TRUE;
                        break;
                    }
                }

                if (!found) {
                    LogError(String_FromCString("Failed to find vulkan instance extension '%s'"), extensionString);
                    hasExtensions = FALSE;
                }
            }
            if (!hasExtensions) goto Error;
        }
        LogDebug(String_FromLiteral("Found all required Vulkan Instance Extensions"));

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

#define VULKAN_INSTANCE_FUNCTION(name) data->name = cast(PFN_ ## name) data->vkGetInstanceProcAddr(data->Instance, #name); if (data->name == nil) { LogDebug(String_FromLiteral("Failed to get Vulkan Instance Function '" #name "'")); goto Error; }
    VULKAN_INSTANCE_FUNCTIONS
#undef VULKAN_INSTANCE_FUNCTION
    LogDebug(String_FromLiteral("Got Vulkan Instance Functions"));

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

    {
        u32 physicalDevicesCount = 0;
        VK_CHECK(data->vkEnumeratePhysicalDevices(data->Instance, &physicalDevicesCount, nil));
        VkPhysicalDevice physicalDevices[physicalDevicesCount];
        VK_CHECK(data->vkEnumeratePhysicalDevices(data->Instance, &physicalDevicesCount, physicalDevices));

        VulkanPhysicalDevice chosenPhysicalDevice = {};
        chosenPhysicalDevice.Device = VK_NULL_HANDLE;

        for (u64 i = 0; i < physicalDevicesCount; i++) {
            VulkanPhysicalDevice physicalDevice = {};
            physicalDevice.Device = physicalDevices[i];
            data->vkGetPhysicalDeviceProperties(physicalDevices[i], &physicalDevice.Properties);
            physicalDevice.DeviceName = String_FromCString(physicalDevice.Properties.deviceName);

            LogDebug(String_FromLiteral("Checking device '%s'..."), physicalDevice.DeviceName);

            if (physicalDevice.Properties.apiVersion < RequiredAPIVersion) {
                LogWarn(String_FromLiteral("Physical Device '%s' does not have the required api version!"), physicalDevice.DeviceName);
                continue;
            }
            LogDebug(String_FromLiteral("Physical Device '%s' has the required api version"), physicalDevice.DeviceName);

            {
                u32 availableLayerCount = 0;
                VK_CHECK(data->vkEnumerateDeviceLayerProperties(physicalDevice.Device, &availableLayerCount, nil));
                VkLayerProperties availableLayers[availableLayerCount];
                VK_CHECK(data->vkEnumerateDeviceLayerProperties(physicalDevice.Device, &availableLayerCount, availableLayers));

                b8 hasLayers = TRUE;
                for (u64 i = 0; i< (sizeof(DeviceLayers) / sizeof(DeviceLayers[0])); i++) {
                    b8 found = FALSE;
                    String layerString = String_FromCString(DeviceLayers[i]);

                    for (u64 j = 0; j < availableLayerCount; j++) {
                        String availableLayerString = String_FromCString(availableLayers[j].layerName);
                        if (String_Equal(layerString, availableLayerString)) {
                            found = TRUE;
                            break;
                        }
                    }

                    if (!found) {
                        LogError(String_FromCString("Physical Device '%s' does not have the layer '%s'"), physicalDevice.DeviceName, layerString);
                        hasLayers = FALSE;
                    }
                }
                if (!hasLayers) continue;
            }
            LogDebug(String_FromLiteral("Physical Device '%s' has the required layers"), physicalDevice.DeviceName);

            {
                u32 availableExtensionCount = 0;
                VK_CHECK(data->vkEnumerateDeviceExtensionProperties(physicalDevice.Device, nil, &availableExtensionCount, nil));
                VkExtensionProperties availableExtensions[availableExtensionCount];
                VK_CHECK(data->vkEnumerateDeviceExtensionProperties(physicalDevice.Device, nil, &availableExtensionCount, availableExtensions));

                b8 hasExtensions = TRUE;
                for (u64 i = 0; i< (sizeof(DeviceExtensions) / sizeof(DeviceExtensions[0])); i++) {
                    b8 found = FALSE;
                    String extensionString = String_FromCString(DeviceExtensions[i]);

                    for (u64 j = 0; j < availableExtensionCount; j++) {
                        String availableExtensionString = String_FromCString(availableExtensions[j].extensionName);
                        if (String_Equal(extensionString, availableExtensionString)) {
                            found = TRUE;
                            break;
                        }
                    }

                    if (!found) {
                        LogError(String_FromCString("Physical Device '%s' does not have the extension '%s'"), physicalDevice.DeviceName, extensionString);
                        hasExtensions = FALSE;
                    }
                }
                if (!hasExtensions) continue;
            }
            LogDebug(String_FromLiteral("Physical Device '%s' has the required extensions"), physicalDevice.DeviceName);

            chosenPhysicalDevice = physicalDevice;

            if (physicalDevice.Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                break;
            }
        }

        if (chosenPhysicalDevice.Device == VK_NULL_HANDLE) {
            LogError(String_FromLiteral("Unable to find a suitable physical device"));
            goto Error;
        }

        data->PhysicalDevice = chosenPhysicalDevice;

        LogDebug(String_FromLiteral("Chosen physical device: '%s', Vulkan API Version: %u.%u.%u"),
            data->PhysicalDevice.DeviceName,
            VK_VERSION_MAJOR(data->PhysicalDevice.Properties.apiVersion),
            VK_VERSION_MINOR(data->PhysicalDevice.Properties.apiVersion),
            VK_VERSION_PATCH(data->PhysicalDevice.Properties.apiVersion)
        );
    }

    LogDebug(String_FromLiteral("Created Vulkan Renderer"));
    return TRUE;
Error:
    LogError(String_FromLiteral("Failed to create Vulkan Renderer!"));
    VulkanRenderer_Destroy(outRenderer);
    return FALSE;

#undef VK_CHECK
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
            LogTrace(String_FromLiteral("Vulkan Trace: %s"), String_FromCString(pCallbackData->pMessage));
        } break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
            LogTrace(String_FromLiteral("Vulkan Debug: %s"), String_FromCString(pCallbackData->pMessage));
        } break;

        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            LogWarn(String_FromLiteral("Vulkan Warn: %s"), String_FromCString(pCallbackData->pMessage));
        } break;

        default:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            LogError(String_FromLiteral("Vulkan Error: %s"), String_FromCString(pCallbackData->pMessage));
        } break;
    }

    return VK_FALSE;
}
#endif
