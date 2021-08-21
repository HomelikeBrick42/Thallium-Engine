#pragma once

#include "Defines.h"

THALLIUM_API void* Allocate(u64 size);
THALLIUM_API void Deallocate(void* block);
THALLIUM_API void MemoryCopy(void* dest, void* src, u64 size);

THALLIUM_API void* TempAllocate(u64 size);
