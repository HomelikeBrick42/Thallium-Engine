#include <Core/Defines.h>
#include <Core/Surface.h>
#include <Renderer/Renderer.h>

#include <stdio.h>

static void OnCloseCallback(Surface* surface) {
    *(cast(b8*) surface->UserData) = FALSE;
}

static void OnKeyCallback(Surface* surface, KeyCode key, b8 pressed) {
    printf("%s %s\n", String_ToTempCString(KeyCode_ToString(key)), pressed ? "Pressed" : "Released");
}

int main(int argc, char** argv) {
    String name = String_FromLiteral("Test Game");

    Surface surface = {};
    if (!Surface_Create(&surface, name, 640, 480)) {
        return -1;
    }
    
    b8 running = TRUE;

    surface.UserData = &running;
    surface.OnCloseCallback = OnCloseCallback;
    surface.OnKeyCallback = OnKeyCallback;

    Renderer renderer = {};
    if (!Renderer_Create(&renderer, RendererAPI_Vulkan, &surface, name)) {
        return -1;
    }

    while (running) {
        Surface_Update(&surface);
    }

    Renderer_Destroy(&renderer);
    Surface_Destroy(&surface);
    return 0;
}
