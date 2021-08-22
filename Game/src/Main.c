#include <Core/Defines.h>
#include <Core/Surface.h>
#include <Core/Logger.h>
#include <Renderer/Renderer.h>

typedef struct GameData {
    String Name;
    b8 Running;
    Surface SurfaceVulkan;
    Renderer RendererVulkan;
    Surface SurfaceOpenGL;
    Renderer RendererOpenGL;
} GameData;

static void OnCloseCallback(Surface* surface) {
    GameData* data = cast(GameData*) surface->UserData;
    data->Running = FALSE;
}

static void OnKeyCallback(Surface* surface, KeyCode key, b8 pressed) {
    LogTrace(String_FromLiteral("%s %s"),
        KeyCode_ToString(key),
        pressed ? String_FromLiteral("Pressed") : String_FromLiteral("Released")
    );
}

static void OnResizeCallback(Surface* surface, u32 width, u32 height) {
    GameData* data = cast(GameData*) surface->UserData;
    Renderer* renderer = surface == &data->SurfaceVulkan ? &data->RendererVulkan : &data->RendererOpenGL;
    Renderer_OnSurfaceResize(renderer, width, height);
}

int main(int argc, char** argv) {
    GameData data = {};
    data.Name = String_FromLiteral("Test Game");

    if (!Surface_Create(&data.SurfaceVulkan, String_FromLiteral("Vulkan Renderer"), 640, 480)) {
        return -1;
    }

    if (!Surface_Create(&data.SurfaceOpenGL, String_FromLiteral("OpenGL Renderer"), 640, 480)) {
        return -1;
    }

    data.Running = TRUE;

    data.SurfaceOpenGL.UserData = &data;
    data.SurfaceOpenGL.OnCloseCallback = OnCloseCallback;
    data.SurfaceOpenGL.OnKeyCallback = OnKeyCallback;
    data.SurfaceOpenGL.OnResizeCallback = OnResizeCallback;

    data.SurfaceVulkan.UserData = &data;
    data.SurfaceVulkan.OnCloseCallback = OnCloseCallback;
    data.SurfaceVulkan.OnKeyCallback = OnKeyCallback;
    data.SurfaceVulkan.OnResizeCallback = OnResizeCallback;

    if (!Renderer_Create(&data.RendererVulkan, RendererAPI_Vulkan, &data.SurfaceVulkan, data.Name)) {
        return -1;
    }

    if (!Renderer_Create(&data.RendererOpenGL, RendererAPI_OpenGL, &data.SurfaceOpenGL, data.Name)) {
        return -1;
    }

    while (data.Running) {
        if (Renderer_BeginFrame(&data.RendererVulkan, 0.0f)) {
            if (!Renderer_EndFrame(&data.RendererVulkan, 0.0f)) {
                LogFatal(String_FromLiteral("Failed to end frame. Shutting Down..."));
                return -1;
            }
        }

        if (Renderer_BeginFrame(&data.RendererOpenGL, 0.0f)) {
            if (!Renderer_EndFrame(&data.RendererOpenGL, 0.0f)) {
                LogFatal(String_FromLiteral("Failed to end frame. Shutting Down..."));
                return -1;
            }
        }

        Surface_Update(&data.SurfaceVulkan);
        Surface_Update(&data.SurfaceOpenGL);
    }

    Renderer_Destroy(&data.RendererVulkan);
    Renderer_Destroy(&data.RendererOpenGL);
    Surface_Destroy(&data.SurfaceVulkan);
    Surface_Destroy(&data.SurfaceOpenGL);
    return 0;
}
