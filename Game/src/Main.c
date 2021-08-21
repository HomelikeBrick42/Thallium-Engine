#include <Core/Defines.h>
#include <Core/Surface.h>

static void OnCloseCallback(Surface* surface, void* userData) {
    *(cast(b8*) userData) = FALSE;
}

int main(int argc, char** argv) {
    Surface surface = {};
    if (!Surface_Create(&surface, String_FromLiteral("Surface"), 640, 480)) {
        return -1;
    }
    
    b8 running = TRUE;

    surface.UserData = &running;
    surface.OnCloseCallback = OnCloseCallback;

    while (running) {
        Surface_Update(&surface);
    }

    Surface_Destroy(&surface);
    return 0;
}
