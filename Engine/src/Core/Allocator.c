#include "Core/Allocator.h"

#include <stdlib.h>
#include <memory.h>

void* Allocate(u64 size) {
    return calloc(1, size);
}

void Deallocate(void* block) {
    if (block == nil) {
        return;
    }

    free(block);
}

void MemoryCopy(void* dest, void* src, u64 size) {
    memmove(dest, src, size);
}

// TODO: Make this per-thread
#define TempAllocatorBlockSize (1024 * 1024 * 4)
static u8 TempAllocatorBlock[TempAllocatorBlockSize];
static u64 TempAllocatorIndex = 0;

THALLIUM_API void* TempAllocate(u64 size) {
    if (size > TempAllocatorBlockSize) {
        return nil;
    }

    if ((TempAllocatorBlockSize - size - 1) < TempAllocatorIndex) {
        TempAllocatorIndex = 0;
    }

    void* block = &TempAllocatorBlock[TempAllocatorIndex];
    TempAllocatorIndex += size;
    memset(block, 0, size);
    return block;
}
