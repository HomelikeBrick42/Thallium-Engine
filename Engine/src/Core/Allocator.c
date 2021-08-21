#include "Core/Allocator.h"

#include <stdlib.h>

void* Allocate(u64 size) {
    return calloc(1, size);
}

void Deallocate(void* block) {
    if (block == nil) {
        return;
    }

    free(block);
}
