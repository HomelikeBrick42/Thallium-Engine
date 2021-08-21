#include <Core/Defines.h>
#include <Core/Surface.h>

static b8 Running = TRUE;

static void OnCloseCallback(Surface* surface) {
    Running = FALSE;
}

int main(int argc, char** argv) {
    Surface surface = {};
    if (!Surface_Create(&surface, "Surface", 640, 480)) {
        return -1;
    }

    surface.OnCloseCallback = OnCloseCallback;

    while (Running) {
        Surface_Update(&surface);
    }

    Surface_Destroy(&surface);
    return 0;
}
