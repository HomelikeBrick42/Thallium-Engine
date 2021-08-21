#include "Containers/String.h"

#include "Core/Allocator.h"

char* String_ToTempCString(String s) {
    char* cstring = TempAllocate(s.Length + 1);
    for (u64 i = 0; i < s.Length; i++) {
        cstring[i] = cast(char) s.Data[i];
    }
    cstring[s.Length] = '\0';
    return cstring;
}
