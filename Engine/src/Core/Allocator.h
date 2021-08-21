#pragma once

#include "Defines.h"

THALLIUM_API void* Allocate(u64 size);
THALLIUM_API void Deallocate(void* block);
