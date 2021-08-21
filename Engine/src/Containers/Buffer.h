#pragma once

#include "Core/Defines.h"

enum {
    BufferElement_ElementSize,
    BufferElement_Capacity,
    BufferElement_Length,
    BufferElement_Count,
};

#define Buffer_Create(type) \
    (cast(type*) _Buffer_Create(sizeof(type)))
THALLIUM_API void* _Buffer_Create(u64 elementSize);

#define Buffer_Destroy(buffer) \
    (cast(void) (_Buffer_Destroy(buffer), buffer = nil))
THALLIUM_API void _Buffer_Destroy(void* buffer);

#define Buffer_Push(buffer, value) \
    do { \
        __typeof__(*buffer) temp = value; \
        buffer = _Buffer_Push(buffer, &temp); \
    } while (0)
THALLIUM_API void* _Buffer_Push(void* buffer, void* value);

#define Buffer_Length(buffer) \
    (((cast(u64*) buffer) - BufferElement_Count)[BufferElement_Length])
