#include <Core/Defines.h>
#include <Core/Surface.h>

int main(int argc, char** argv) {
    Surface surface = {};
    if (!Surface_Create(&surface, "Surface", 640, 480)) {
        return -1;
    }

    while (TRUE) {
        Surface_Update(&surface);
    }

    Surface_Destroy(&surface);
    return 0;
}
