#include <Core/Defines.h>
#include <Core/Surface.h>

#include <stdio.h>

static void OnCloseCallback(Surface* surface) {
    *(cast(b8*) surface->UserData) = FALSE;
}

static void OnKeyCallback(Surface* surface, KeyCode key, b8 pressed) {
    printf("%s %s\n", String_ToTempCString(KeyCode_ToString(key)), pressed ? "Pressed" : "Released");
}

int main(int argc, char** argv) {
    Surface surface = {};
    if (!Surface_Create(&surface, String_FromLiteral("Surface"), 640, 480)) {
        return -1;
    }
    
    b8 running = TRUE;

    surface.UserData = &running;
    surface.OnCloseCallback = OnCloseCallback;
    surface.OnKeyCallback = OnKeyCallback;

    while (running) {
        Surface_Update(&surface);
    }

    Surface_Destroy(&surface);
    return 0;
}
