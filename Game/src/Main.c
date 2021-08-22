#include <Core/Defines.h>
#include <Core/Surface.h>
#include <Core/Logger.h>
#include <Renderer/Renderer.h>

typedef struct GameData {
    String Name;
    b8 Running;
    Surface Surface;
    Renderer Renderer;
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
    Renderer_OnSurfaceResize(&data->Renderer, width, height);
    LogTrace(String_FromLiteral("Resize: %u, %u"), width, height);
}

int main(int argc, char** argv) {
    GameData data = {};
    data.Name = String_FromLiteral("Test Game");

    if (!Surface_Create(&data.Surface, data.Name, 640, 480)) {
        return -1;
    }

    data.Running = TRUE;

    data.Surface.UserData = &data;
    data.Surface.OnCloseCallback = OnCloseCallback;
    data.Surface.OnKeyCallback = OnKeyCallback;
    data.Surface.OnResizeCallback = OnResizeCallback;

    if (!Renderer_Create(&data.Renderer, RendererAPI_Vulkan, &data.Surface, data.Name)) {
        return -1;
    }

    while (data.Running) {
        if (Renderer_BeginFrame(&data.Renderer, 0.0f)) {
            if (!Renderer_EndFrame(&data.Renderer, 0.0f)) {
                LogFatal(String_FromLiteral("Failed to end frame. Shutting Down..."));
                return -1;
            }
        }

        Surface_Update(&data.Surface);
    }

    Renderer_Destroy(&data.Renderer);
    Surface_Destroy(&data.Surface);
    return 0;
}
