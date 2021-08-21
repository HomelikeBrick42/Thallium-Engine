#include "Containers/Buffer.h"

#include "Core/Allocator.h"

void* _Buffer_Create(u64 elementSize) {
    u64* header = Allocate(BufferElement_Count * sizeof(u64));
    header[BufferElement_ElementSize] = elementSize;
    header[BufferElement_Capacity] = 0;
    header[BufferElement_Length] = 0;
    return header + BufferElement_Count;
}

void _Buffer_Destroy(void* buffer) {
    u64* header = (cast(u64*) buffer) - BufferElement_Count;
    Deallocate(header);
}

void* _Buffer_Push(void* buffer, void* value) {
    u64* header = (cast(u64*) buffer) - BufferElement_Count;
    if (header[BufferElement_Length] <= header[BufferElement_Capacity]) {
        u64 newCapacity = header[BufferElement_Capacity] == 0 ? 1 : header[BufferElement_Capacity] * 2;
        u64* newHeader = Allocate((BufferElement_Count * sizeof(u64)) + (newCapacity * header[BufferElement_ElementSize]));
        MemoryCopy(newHeader, header, (BufferElement_Count * sizeof(u64)) + (header[BufferElement_Length] * header[BufferElement_ElementSize]));

        Deallocate(header);
        header = newHeader;
        header[BufferElement_Capacity] = newCapacity;
        buffer = header + BufferElement_Count;
    }

    MemoryCopy((cast(u8*) buffer) + (header[BufferElement_Length] * header[BufferElement_ElementSize]), value, header[BufferElement_ElementSize]);
    header[BufferElement_Length]++;

    return buffer;
}
